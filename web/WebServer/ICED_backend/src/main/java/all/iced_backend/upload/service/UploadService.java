package all.iced_backend.upload.service;

import all.iced_backend.upload.dto.UploadResponseDto;
import all.iced_backend.upload.entity.UploadFile;
import all.iced_backend.upload.repository.UploadFileRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.IOException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.concurrent.CompletableFuture;

@Service
@RequiredArgsConstructor
public class UploadService {

    // FTPS 서버 설정 (application.properties에서 주입)
    @Value("${ftps.host}")
    private String ftpsHost;
    @Value("${ftps.port}")
    private int ftpsPort;
    @Value("${ftps.username}")
    private String ftpsUser;
    @Value("${ftps.password}")
    private String ftpsPass;
    @Value("${ftps.remote-dir}")
    private String ftpsRemoteDir;

    private final UploadFileRepository uploadFileRepository;

    // FTPS 업로더 유틸
    private final FtpsUploader ftpsUploader = new FtpsUploader();
    /**
     * 비동기(Async) 방식으로 파일 업로드
     */
    @Async
    public CompletableFuture<UploadResponseDto> uploadFile(MultipartFile file, MultipartFile thumbnail,
                                                           String uploaderId, String description) {
        CompletableFuture<UploadResponseDto> future = new CompletableFuture<>();

        try {
            UploadResponseDto result = performUpload(file, thumbnail, uploaderId, description);
            future.complete(result);
        } catch (Exception e) {
            e.printStackTrace();
            future.completeExceptionally(e);
        }

        return future;
    }

    private UploadResponseDto performUpload(MultipartFile file, MultipartFile thumbnail,
                                            String uploaderId, String description) throws IOException {
        String originalFilename = file.getOriginalFilename();
        if (originalFilename == null || originalFilename.isEmpty()) {
            throw new IllegalArgumentException("Invalid file name.");
        }

        double sizeInMB = file.getSize() / (1024.0 * 1024.0);   // 파일 크기 계산
        BigDecimal roundedSize = new BigDecimal(sizeInMB).setScale(2, RoundingMode.HALF_UP);
        double finalSize = roundedSize.doubleValue();

        File tempFile = null;
        File tempThumbnail = null;
        try {
            // 1) 임시 파일 생성
            String prefix = "upload_" + System.currentTimeMillis() + "_";
            tempFile = File.createTempFile(prefix, "_" + originalFilename);
            file.transferTo(tempFile);

            // 2) FTPS 서버에 폴더 생성 (동기 처리)
            String modelDir = ftpsRemoteDir + "/models";
            String thumbnailDir = ftpsRemoteDir + "/thumbnails";
            if (!ftpsUploader.ensureDirectoryExists(ftpsHost, ftpsPort, ftpsUser, ftpsPass, modelDir)) {
                throw new RuntimeException("모델 디렉토리 생성 실패");
            }
            if (thumbnail != null && !thumbnail.isEmpty() &&
                    !ftpsUploader.ensureDirectoryExists(ftpsHost, ftpsPort, ftpsUser, ftpsPass, thumbnailDir)) {
                throw new RuntimeException("썸네일 디렉토리 생성 실패");
            }

            // 2) FTPS 서버에 업로드
            String remotePath = ftpsUploader.uploadFile(
                    ftpsHost, ftpsPort, ftpsUser, ftpsPass,
                    tempFile, ftpsRemoteDir + "/models"
            );

            if (remotePath == null) {
                throw new RuntimeException("FTPS 업로드 실패");
            }

            // 3) 썸네일 처리
            String thumbnailPath = null;
            if (thumbnail != null && !thumbnail.isEmpty()) {
                tempThumbnail = File.createTempFile("thumbnail_", "_" + thumbnail.getOriginalFilename());
                thumbnail.transferTo(tempThumbnail);
                thumbnailPath = ftpsUploader.uploadFile(
                        ftpsHost, ftpsPort, ftpsUser, ftpsPass, tempThumbnail, ftpsRemoteDir + "/thumbnails");
                if (thumbnailPath == null) {
                    throw new RuntimeException("썸네일 업로드 실패");
                }
            }

            // 4) DB 저장
            UploadFile uploadFile = new UploadFile();
            uploadFile.setFileName(originalFilename);
            uploadFile.setFileUri(remotePath);
            uploadFile.setUploader_id(uploaderId);
            uploadFile.setDescription(description);
            uploadFile.setSize(finalSize);
            uploadFile.setThumbnailUri(thumbnailPath);
            UploadFile saved = uploadFileRepository.save(uploadFile);

            // 5) 응답 DTO 생성 후 반환
            return new UploadResponseDto(
                    saved.getId(), saved.getFileName(), saved.getFileUri(),
                    saved.getUploader_id(), saved.getDescription(), saved.getSize(), saved.getThumbnailUri()
            );

        } finally {
            // 6) 임시 파일 삭제
            if (tempFile != null && tempFile.exists() && !tempFile.delete()) {
                System.err.println("임시 파일 삭제 실패: " + tempFile.getAbsolutePath());
            }
            if (tempThumbnail != null && tempThumbnail.exists() && !tempThumbnail.delete()) {
                System.err.println("임시 썸네일 파일 삭제 실패: " + tempThumbnail.getAbsolutePath());
            }
        }
    }
}

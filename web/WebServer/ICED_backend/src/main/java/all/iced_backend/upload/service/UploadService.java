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
    public CompletableFuture<UploadResponseDto> uploadFile(MultipartFile file) {
        String originalFilename = file.getOriginalFilename();
        if (originalFilename == null || originalFilename.isEmpty()) {
            throw new IllegalArgumentException("Invalid file name.");
        }

        // 1) 임시 파일 생성
        File tempFile;
        try {
            tempFile = File.createTempFile("upload_", "_" + originalFilename);
            file.transferTo(tempFile);
        } catch (IOException e) {
            throw new RuntimeException("임시 파일 생성 실패: " + e.getMessage());
        }

        // 2) FTPS 서버에 업로드
        String remotePath = ftpsUploader.uploadFile(
                ftpsHost, ftpsPort, ftpsUser, ftpsPass,
                tempFile, ftpsRemoteDir
        );
        // 임시 파일 삭제
        tempFile.delete();

        if (remotePath == null) {
            throw new RuntimeException("FTPS 업로드 실패");
        }

        // 3) DB 저장
        UploadFile uploadFile = new UploadFile();
        uploadFile.setFileName(originalFilename);
        uploadFile.setFileUri(remotePath);
        UploadFile saved = uploadFileRepository.save(uploadFile);

        // 4) DTO 반환
        UploadResponseDto dto = new UploadResponseDto(saved.getId(), saved.getFileName(), saved.getFileUri());
        return CompletableFuture.completedFuture(dto);
    }
}

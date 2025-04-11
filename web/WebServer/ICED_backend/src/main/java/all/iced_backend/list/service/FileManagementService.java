package all.iced_backend.list.service;

import all.iced_backend.list.dto.FileUpdateRequestDto;
import all.iced_backend.list.dto.FileUpdateResponseDto;
import all.iced_backend.list.entity.FileList;
import all.iced_backend.list.repository.FileListRepository;
import all.iced_backend.upload.service.FtpsUploader;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.access.AccessDeniedException;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.IOException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.time.LocalDateTime;

@Service
@RequiredArgsConstructor
public class FileManagementService {

    // FTPS 서버 설정
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

    private final FileListRepository fileListRepository;
    private final FtpsUploader ftpsUploader = new FtpsUploader();
    private final FtpsFileManager ftpsFileManager = new FtpsFileManager();

    private void checkFileOwnership(FileList file, String userId) {
        if (userId == null || !file.getUploader_id().equals(userId)) {
            throw new AccessDeniedException("이 파일에 대한 권한이 없습니다. 본인이 업로드한 파일만 수정/삭제할 수 있습니다.");
        }
    }

    /**
     * 파일 정보 수정 (권한 검사 추가)
     */
    @Transactional
    public FileUpdateResponseDto updateFile(Long fileId, FileUpdateRequestDto updateRequest, String userId) {
        // 1. 파일 존재 확인
        FileList file = fileListRepository.findById(fileId)
                .orElseThrow(() -> new RuntimeException("파일을 찾을 수 없습니다: " + fileId));

        // 2. 권한 검사 추가
        checkFileOwnership(file, userId);

        // 3. 설명 업데이트 (설명이 제공된 경우)
        if (updateRequest.getDescription() != null && !updateRequest.getDescription().isEmpty()) {
            file.setDescription(updateRequest.getDescription());
        }

        // 4. DB 업데이트 및 응답 반환
        FileList savedFile = fileListRepository.save(file);

        return FileUpdateResponseDto.builder()
                .id(savedFile.getId())
                .fileName(savedFile.getFileName())
                .description(savedFile.getDescription())
                .thumbnailUri(savedFile.getThumbnailUri())
                .fileUri(savedFile.getFileUri())
                .size(savedFile.getSize())
                .createdAt(LocalDateTime.now())
                .build();
    }

    /**
     * 파일 수정 (파일 교체, 썸네일 교체 가능) (권한 검사 추가)
     */
    @Transactional
    public FileUpdateResponseDto updateFileWithContent(
            Long fileId,
            MultipartFile newFile,
            MultipartFile newThumbnail,
            String description,
            String userId) throws IOException {

        // 1. 파일 존재 확인
        FileList file = fileListRepository.findById(fileId)
                .orElseThrow(() -> new RuntimeException("파일을 찾을 수 없습니다: " + fileId));

        // 2. 권한 검사 추가
        checkFileOwnership(file, userId);

        // 3. 모델 파일 업데이트 (새 파일이 제공된 경우)
        if (newFile != null && !newFile.isEmpty()) {
            // 기존 파일 삭제
            if (file.getFileUri() != null) {
                ftpsFileManager.deleteFile(ftpsHost, ftpsPort, ftpsUser, ftpsPass, file.getFileUri());
            }

            // 새 파일 업로드
            File tempFile = File.createTempFile("update_", "_" + newFile.getOriginalFilename());
            try {
                newFile.transferTo(tempFile);

                // 모델 디렉토리 존재 확인
                String modelDir = ftpsRemoteDir + "/models";
                ftpsUploader.ensureDirectoryExists(ftpsHost, ftpsPort, ftpsUser, ftpsPass, modelDir);

                // 파일 업로드
                String remotePath = ftpsUploader.uploadFile(
                        ftpsHost, ftpsPort, ftpsUser, ftpsPass, tempFile, modelDir);

                if (remotePath != null) {
                    file.setFileUri(remotePath);
                    file.setFileName(newFile.getOriginalFilename());

                    // 파일 크기 계산 및 업데이트
                    double sizeInMB = newFile.getSize() / (1024.0 * 1024.0);
                    BigDecimal roundedSize = new BigDecimal(sizeInMB).setScale(2, RoundingMode.HALF_UP);
                    file.setSize(roundedSize.doubleValue());
                }
            } finally {
                if (tempFile.exists()) {
                    tempFile.delete();
                }
            }
        }

        // 4. 썸네일 업데이트 (새 썸네일이 제공된 경우)
        if (newThumbnail != null && !newThumbnail.isEmpty()) {
            // 기존 썸네일 삭제
            if (file.getThumbnailUri() != null) {
                ftpsFileManager.deleteFile(ftpsHost, ftpsPort, ftpsUser, ftpsPass, file.getThumbnailUri());
            }

            // 새 썸네일 업로드
            File tempThumbnail = File.createTempFile("thumb_update_", "_" + newThumbnail.getOriginalFilename());
            try {
                newThumbnail.transferTo(tempThumbnail);

                // 썸네일 디렉토리 존재 확인
                String thumbnailDir = ftpsRemoteDir + "/thumbnails";
                ftpsUploader.ensureDirectoryExists(ftpsHost, ftpsPort, ftpsUser, ftpsPass, thumbnailDir);

                // 썸네일 업로드
                String thumbnailPath = ftpsUploader.uploadFile(
                        ftpsHost, ftpsPort, ftpsUser, ftpsPass, tempThumbnail, thumbnailDir);

                if (thumbnailPath != null) {
                    file.setThumbnailUri(thumbnailPath);
                }
            } finally {
                if (tempThumbnail.exists()) {
                    tempThumbnail.delete();
                }
            }
        }

        // 5. 설명 업데이트 (설명이 제공된 경우)
        if (description != null && !description.isEmpty()) {
            file.setDescription(description);
        }

        // 6. DB 업데이트 및 응답 반환
        FileList savedFile = fileListRepository.save(file);

        return FileUpdateResponseDto.builder()
                .id(savedFile.getId())
                .fileName(savedFile.getFileName())
                .description(savedFile.getDescription())
                .thumbnailUri(savedFile.getThumbnailUri())
                .fileUri(savedFile.getFileUri())
                .size(savedFile.getSize())
                .createdAt(LocalDateTime.now())
                .build();
    }

    /**
     * 파일 삭제 (권한 검사 추가)
     */
    @Transactional
    public boolean deleteFile(Long fileId, String userId) {
        // 1. 파일 존재 확인
        FileList file = fileListRepository.findById(fileId)
                .orElseThrow(() -> new RuntimeException("파일을 찾을 수 없습니다: " + fileId));

        // 2. 권한 검사 추가
        checkFileOwnership(file, userId);

        // 3. FTPS 서버에서 모델 파일 삭제
        boolean modelDeleted = true;
        if (file.getFileUri() != null) {
            modelDeleted = ftpsFileManager.deleteFile(ftpsHost, ftpsPort, ftpsUser, ftpsPass, file.getFileUri());
        }

        // 4. FTPS 서버에서 썸네일 파일 삭제
        boolean thumbnailDeleted = true;
        if (file.getThumbnailUri() != null) {
            thumbnailDeleted = ftpsFileManager.deleteFile(ftpsHost, ftpsPort, ftpsUser, ftpsPass, file.getThumbnailUri());
        }

        // 5. DB에서 파일 정보 삭제
        fileListRepository.deleteById(fileId);

        // 6. 삭제 성공 여부 반환 (모든 파일이 삭제되었는지 확인)
        return modelDeleted && thumbnailDeleted;
    }
}

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

    // application.properties에 설정된 공유 폴더 경로 (예: Z:/Uploads)
    @Value("${app.upload-dir}")
    private String uploadDir;

    private final UploadFileRepository uploadFileRepository;

    @Async
    public CompletableFuture<UploadResponseDto> uploadFileAsync(MultipartFile file) {
        String originalFilename = file.getOriginalFilename();
        if (originalFilename == null || originalFilename.isEmpty()) {
            throw new IllegalArgumentException("유효한 파일명이 아닙니다.");
        }

        File folder = new File(uploadDir);
        if (!folder.exists()) {
            folder.mkdirs();
        }

        File destination = new File(folder, originalFilename);
        try {
            file.transferTo(destination);
        } catch (IOException e) {
            throw new RuntimeException("파일 업로드 실패: " + e.getMessage());
        }

        UploadFile uploadFile = new UploadFile();
        uploadFile.setFileName(originalFilename);
        uploadFile.setFileUri(destination.getAbsolutePath());
        UploadFile saved = uploadFileRepository.save(uploadFile);

        UploadResponseDto responseDto = new UploadResponseDto(saved.getId(), saved.getFileName(), saved.getFileUri());
        return CompletableFuture.completedFuture(responseDto);
    }
}

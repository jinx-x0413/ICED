package all.iced_backend.upload.service;

import all.iced_backend.upload.dto.UploadResponseDto;
import all.iced_backend.upload.entity.UploadFile;
import all.iced_backend.upload.repository.UploadFileRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.IOException;

@Service
@RequiredArgsConstructor
public class UploadService {

    // application.properties에 설정된 공유 폴더 경로 (예: Z:/Uploads)
    @Value("${app.upload-dir}")
    private String uploadDir;

    private final UploadFileRepository uploadFileRepository;

    public UploadResponseDto uploadFile(MultipartFile file) {
        String originalFilename = file.getOriginalFilename();
        if(originalFilename == null || originalFilename.isEmpty()){
            throw new IllegalArgumentException("유효한 파일명이 아닙니다.");
        }

        // 공유 폴더 (마운트된 경로)가 존재하지 않으면 생성
        File folder = new File(uploadDir);
        if (!folder.exists()) {
            folder.mkdirs();
        }

        // 파일 저장
        File destination = new File(folder, originalFilename);
        try {
            file.transferTo(destination);
        } catch (IOException e) {
            throw new RuntimeException("파일 업로드 실패: " + e.getMessage());
        }

        // DB에 파일 메타데이터 저장
        UploadFile uploadFile = new UploadFile();
        uploadFile.setFileName(originalFilename);
        uploadFile.setFileUri(destination.getAbsolutePath());
        UploadFile saved = uploadFileRepository.save(uploadFile);

        return new UploadResponseDto(saved.getId(), saved.getFileName(), saved.getFileUri());
    }
}

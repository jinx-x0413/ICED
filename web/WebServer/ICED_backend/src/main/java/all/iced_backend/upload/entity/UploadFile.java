package all.iced_backend.upload.entity;

import lombok.Getter;
import lombok.Setter;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Getter
@Setter
@Table(name = "test_upload_files")
public class UploadFile {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    // 원본 파일명
    private String fileName;

    // FTPS 서버에 저장된 경로 (ex: "/uploads/Bed.fbx")
    private String fileUri;

    // 업로드된 시간
    private LocalDateTime createdAt = LocalDateTime.now();
}

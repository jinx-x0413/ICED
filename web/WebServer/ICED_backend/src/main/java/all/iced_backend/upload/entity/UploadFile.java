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

    // 업로드된 파일 이름
    @Column(name = "file_name", nullable = false)
    private String fileName;

    // 파일 저장 경로 (또는 URL)
    @Column(name = "file_uri", nullable = false)
    private String fileUri;

    // 업로드 시각
    @Column(name = "created_at")
    private LocalDateTime createdAt = LocalDateTime.now();
}

package all.iced_backend.list.entity;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDateTime;

@Entity
@Table(name = "test_upload_files") // 기존 테이블과 동일한 테이블 사용
@Getter @Setter
@NoArgsConstructor
@AllArgsConstructor
public class FileList {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String fileName;

    private String fileUri;

    private String uploader_id;

    private String description;

    private Double size;

    private String createdAt;

    private String thumbnailUri;
}

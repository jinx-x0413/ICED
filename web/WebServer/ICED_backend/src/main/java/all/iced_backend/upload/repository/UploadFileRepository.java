package all.iced_backend.upload.repository;

import all.iced_backend.upload.entity.UploadFile;
import org.springframework.data.jpa.repository.JpaRepository;

public interface UploadFileRepository extends JpaRepository<UploadFile, Long> {
    // 추가 쿼리 메서드가 필요하면 여기서 정의
}

package all.iced_backend.list.repository;

import all.iced_backend.list.entity.FileList;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface FileListRepository extends JpaRepository<FileList, Long> {
    // JPQL 쿼리를 사용해 uploader_id 필드로 조회
    @Query("SELECT f FROM FileList f WHERE f.uploader_id = :uploaderId")
    List<FileList> findByUploaderId(@Param("uploaderId") String uploaderId);

    // 최신순(createdAt 기준 내림차순) 정렬 조회
    @Query("SELECT f FROM FileList f ORDER BY f.createdAt DESC")
    List<FileList> findAllOrderByCreatedAtDesc();
}

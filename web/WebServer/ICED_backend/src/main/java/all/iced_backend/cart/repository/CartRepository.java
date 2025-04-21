package all.iced_backend.cart.repository;

import all.iced_backend.cart.entity.CartItem;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;

@Repository
public interface CartRepository extends JpaRepository<CartItem, Long> {

    // 사용자 ID로 장바구니 항목 조회 (정렬: 추가일 내림차순)
    List<CartItem> findByUserIdOrderByAddedAtDesc(String userId);

    // 사용자 ID 및 파일 ID로 장바구니 항목 조회
    Optional<CartItem> findByUserIdAndFileId(String userId, Long fileId);

    // 사용자 ID 및 파일 ID로 장바구니 항목 삭제
    @Modifying
    @Query("DELETE FROM CartItem c WHERE c.userId = :userId AND c.fileId = :fileId")
    void deleteByUserIdAndFileId(@Param("userId") String userId, @Param("fileId") Long fileId);

    // 사용자의 모든 장바구니 항목 삭제
    @Modifying
    @Query("DELETE FROM CartItem c WHERE c.userId = :userId")
    void deleteAllByUserId(@Param("userId") String userId);
}

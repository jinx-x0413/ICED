package all.iced_backend.auth.repository;

import all.iced_backend.auth.entity.RefreshEntity;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.transaction.annotation.Transactional;

public interface RefreshRepository extends JpaRepository<RefreshEntity, Long> {

    Boolean existsByRefresh(String refresh);

    @Transactional
    void deleteByUserid(String userid);
    @Transactional
    void deleteByRefresh(String refresh);
}

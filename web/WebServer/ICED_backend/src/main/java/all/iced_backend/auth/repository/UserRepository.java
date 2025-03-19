package all.iced_backend.auth.repository;

import all.iced_backend.auth.entity.Users;
import org.springframework.data.jpa.repository.JpaRepository;

public interface UserRepository extends JpaRepository<Users,Integer> {

    Boolean existsByUserid(String userid);

    Users findByUserid(String userid);
}

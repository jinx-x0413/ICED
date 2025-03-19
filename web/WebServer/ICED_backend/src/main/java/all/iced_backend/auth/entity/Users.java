package all.iced_backend.auth.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;

@Entity
@Setter
@Getter
public class Users {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int Uid;

    private String userid; // id 컬럼에 해당

    private String role; // role 컬럼에 해당

    private String email; // email 컬럼에 해당

    private String password; // password 컬럼에 해당

    private String username; // username 컬럼에 해당

    private LocalDateTime createdAt; // created_at 컬럼에 해당
}
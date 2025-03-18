package all.iced_backend.auth.dto;


import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;

@Getter
@Setter
public class JoinDTO {

    private String userid; // id 컬럼에 해당

    private String email; // email 컬럼에 해당

    private String password; // password 컬럼에 해당

    private String username; // username 컬럼에 해당

    private LocalDateTime createdAt; // createdat 컬럼에 해당
}

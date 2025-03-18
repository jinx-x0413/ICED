package all.iced_backend.auth.service;

import all.iced_backend.auth.dto.JoinDTO;
import all.iced_backend.auth.entity.Users;
import all.iced_backend.auth.repository.UserRepository;

import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;

@Service
public class JoinService {

    private final UserRepository userRepository;
    private final BCryptPasswordEncoder bCryptPasswordEncoder;


    public JoinService(UserRepository userRepository, BCryptPasswordEncoder bCryptPasswordEncoder) {

        this.userRepository = userRepository;
        this.bCryptPasswordEncoder = bCryptPasswordEncoder;
    }

    public String joinProcess(JoinDTO joinDTO){

        String userid = joinDTO.getUserid();
        String email = joinDTO.getEmail();
        String username = joinDTO.getUsername();
        String password = joinDTO.getPassword();


        Boolean isExist = userRepository.existsByUserid(userid);

        if(isExist){
            return "id가 이미 존재합니다";
        }

        Users data = new Users();

        data.setEmail(email);
        data.setUserid(userid);
        data.setUsername(username);
        data.setPassword(bCryptPasswordEncoder.encode(password));
        data.setCreatedAt(LocalDateTime.now());

        userRepository.save(data);

        return "회원가입 성공";
    }
}

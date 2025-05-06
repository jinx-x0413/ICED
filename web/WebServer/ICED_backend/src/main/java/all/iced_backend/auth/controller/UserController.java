package all.iced_backend.auth.controller;

import all.iced_backend.auth.dto.UserInfoResponse;
import all.iced_backend.auth.entity.Users;
import all.iced_backend.auth.jwt.JWTUtil;
import all.iced_backend.auth.repository.UserRepository;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/userinfo")
public class UserController {

    private final JWTUtil jwtUtil;
    private final UserRepository userRepository;

    public UserController(JWTUtil jwtUtil, UserRepository userRepository) {
        this.jwtUtil = jwtUtil;
        this.userRepository = userRepository;
    }

    @GetMapping
    public UserInfoResponse getUserInfo(HttpServletRequest request) {

        // 1. 헤더에서 토큰 추출
        String authorization = request.getHeader("Authorization");
        if (authorization == null || !authorization.startsWith("Bearer ")) {
            throw new RuntimeException("토큰이 유효하지 않습니다.");
        }

        String token = authorization.substring(7); // "Bearer " 제거

        // 2. 토큰에서 userid 추출
        String userid = jwtUtil.getUserid(token);

        // 3. DB에서 해당 유저 조회
        Users user = userRepository.findByUserid(userid);

        if (user == null) {
            throw new RuntimeException("유저를 찾을 수 없습니다.");
        }

        // 4. 필요한 정보만 DTO로 리턴
        return new UserInfoResponse(user.getUserid(), user.getEmail(), user.getUsername());
    }
}

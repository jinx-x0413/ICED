package all.iced_backend.list.controller;

import all.iced_backend.list.dto.FileListResponseDto;
import all.iced_backend.list.service.FileListService;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.security.Keys;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.crypto.SecretKey;
import java.nio.charset.StandardCharsets;
import java.util.List;

@RestController
@RequestMapping("/api/files")
@CrossOrigin(origins = "*")
@RequiredArgsConstructor
public class FileListController {

    private final FileListService fileListService;

    @Value("${Secret_Key}")
    private String secretKey;

    /**
     * JWT 토큰에서 사용자 ID 추출 (JJWT 0.12.3 버전용)
     */
    private String extractUserIdFromToken(String token) {
        try {
            // "Bearer " 접두사 제거
            if (token != null && token.startsWith("Bearer ")) {
                token = token.substring(7);
            }

            // 시크릿 키 생성
            SecretKey key = Keys.hmacShaKeyFor(secretKey.getBytes(StandardCharsets.UTF_8));

            // 토큰 복호화
            Claims claims = Jwts.parser()
                    .verifyWith(key)
                    .build()
                    .parseSignedClaims(token)
                    .getPayload();

            // 'userid' 클레임에서 사용자 ID 추출
            return claims.get("userid", String.class);
        } catch (Exception e) {
            System.err.println("JWT 토큰 파싱 실패: " + e.getMessage());
            return null;
        }
    }

    /**
     * 모든 파일 목록 조회 (최신순)
     */
    @GetMapping
    public ResponseEntity<List<FileListResponseDto>> getAllFiles() {
        return ResponseEntity.ok(fileListService.getAllFiles());
    }

    /**
     * 특정 사용자의 파일 목록 조회 (JWT 토큰 인증 추가)
     */
    @GetMapping("/user/{uploaderId}")
    public ResponseEntity<?> getFilesByUploader(
            @PathVariable String uploaderId,
            @RequestHeader(value = "Authorization", required = false) String authHeader) {

        // 토큰에서 사용자 ID 추출
        String tokenUserId = extractUserIdFromToken(authHeader);
        if (tokenUserId == null) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body("인증 정보가 유효하지 않습니다");
        }

        // 요청한 사용자 ID와 토큰의 사용자 ID가 일치하는지 확인
        if (!tokenUserId.equals(uploaderId)) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body("다른 사용자의 파일 목록을 조회할 권한이 없습니다");
        }

        // 권한 검증 통과 후 파일 목록 조회
        return ResponseEntity.ok(fileListService.getFilesByUploader(uploaderId));
    }

    /**
     * 단일 파일 상세 정보 조회
     */
    @GetMapping("/{fileId}")
    public ResponseEntity<FileListResponseDto> getFileById(@PathVariable Long fileId) {
        try {
            return ResponseEntity.ok(fileListService.getFileById(fileId));
        } catch (RuntimeException e) {
            return ResponseEntity.notFound().build();
        }
    }
}

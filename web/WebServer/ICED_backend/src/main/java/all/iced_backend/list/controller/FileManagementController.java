package all.iced_backend.list.controller;

import all.iced_backend.list.dto.FileUpdateRequestDto;
import all.iced_backend.list.dto.FileUpdateResponseDto;
import all.iced_backend.list.service.FileManagementService;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.security.Keys;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.AccessDeniedException;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.crypto.SecretKey;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

@RestController
@RequestMapping("/api/files")
@CrossOrigin(origins = "*")
@RequiredArgsConstructor
public class FileManagementController {

    private final FileManagementService fileManagementService;

    @Value("${Secret_Key}")
    private String secretKey; // application.properties 또는 application.yml에서 설정

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

            // 토큰 복호화 (0.12.3 버전 API 사용)
            Claims claims = Jwts.parser()
                    .verifyWith(key)
                    .build()
                    .parseSignedClaims(token)
                    .getPayload();

            // 'userid' 클레임에서 사용자 ID 추출
            return claims.get("userid", String.class);
        } catch (Exception e) {
            // 토큰 파싱 실패 로그
            System.err.println("JWT 토큰 파싱 실패: " + e.getMessage());
            return null;
        }
    }

    /**
     * 파일 정보 업데이트 (메타데이터만)
     */
    @PutMapping("/{fileId}")
    public ResponseEntity<?> updateFileInfo(
            @PathVariable Long fileId,
            @RequestBody FileUpdateRequestDto updateRequest,
            @RequestHeader(value = "Authorization", required = false) String authHeader) {

        try {
            // 토큰에서 사용자 ID 추출
            String userId = extractUserIdFromToken(authHeader);
            if (userId == null) {
                return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                        .body("인증 정보가 유효하지 않습니다");
            }

            // 권한 검사가 추가된 서비스 메소드 호출
            FileUpdateResponseDto result = fileManagementService.updateFile(fileId, updateRequest, userId);
            return ResponseEntity.ok(result);

        } catch (AccessDeniedException e) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body(e.getMessage());
        } catch (RuntimeException e) {
            if (e.getMessage().contains("찾을 수 없습니다")) {
                return ResponseEntity.status(HttpStatus.NOT_FOUND)
                        .body(e.getMessage());
            }
            e.printStackTrace();
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("서버 오류: " + e.getMessage());
        }
    }

    /**
     * 파일 업데이트 (파일 콘텐츠 포함)
     */
    @PostMapping("/{fileId}/update")
    public ResponseEntity<?> updateFileContent(
            @PathVariable Long fileId,
            @RequestParam(value = "file", required = false) MultipartFile file,
            @RequestParam(value = "thumbnail", required = false) MultipartFile thumbnail,
            @RequestParam(value = "description", required = false) String description,
            @RequestHeader(value = "Authorization", required = false) String authHeader) {

        try {
            // 토큰에서 사용자 ID 추출
            String userId = extractUserIdFromToken(authHeader);
            if (userId == null) {
                return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                        .body("인증 정보가 유효하지 않습니다");
            }

            // 권한 검사가 추가된 서비스 메소드 호출
            FileUpdateResponseDto result = fileManagementService.updateFileWithContent(
                    fileId, file, thumbnail, description, userId);
            return ResponseEntity.ok(result);

        } catch (AccessDeniedException e) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body(e.getMessage());
        } catch (RuntimeException e) {
            if (e.getMessage().contains("찾을 수 없습니다")) {
                return ResponseEntity.status(HttpStatus.NOT_FOUND)
                        .body(e.getMessage());
            }
            e.printStackTrace();
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("서버 오류: " + e.getMessage());
        } catch (IOException e) {
            e.printStackTrace();
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("파일 처리 중 오류: " + e.getMessage());
        }
    }

    /**
     * 파일 삭제
     */
    @DeleteMapping("/{fileId}")
    public ResponseEntity<?> deleteFile(
            @PathVariable Long fileId,
            @RequestHeader(value = "Authorization", required = false) String authHeader) {

        try {
            // 토큰에서 사용자 ID 추출
            String userId = extractUserIdFromToken(authHeader);
            if (userId == null) {
                return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                        .body("인증 정보가 유효하지 않습니다");
            }

            // 권한 검사가 추가된 서비스 메소드 호출
            boolean deleted = fileManagementService.deleteFile(fileId, userId);

            if (deleted) {
                return ResponseEntity.ok("파일이 성공적으로 삭제되었습니다.");
            } else {
                return ResponseEntity.status(HttpStatus.PARTIAL_CONTENT)
                        .body("파일 정보는 삭제되었으나 일부 파일 삭제에 실패했습니다.");
            }

        } catch (AccessDeniedException e) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body(e.getMessage());
        } catch (RuntimeException e) {
            if (e.getMessage().contains("찾을 수 없습니다")) {
                return ResponseEntity.status(HttpStatus.NOT_FOUND)
                        .body(e.getMessage());
            }
            e.printStackTrace();
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("파일 삭제 중 오류가 발생했습니다: " + e.getMessage());
        }
    }
}

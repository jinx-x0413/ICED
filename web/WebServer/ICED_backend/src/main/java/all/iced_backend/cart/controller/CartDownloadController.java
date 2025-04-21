package all.iced_backend.cart.controller;

import all.iced_backend.cart.service.CartDownloadService;
import all.iced_backend.auth.jwt.JWTUtil;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/cart/download")
@CrossOrigin(origins = "*")
@RequiredArgsConstructor
@Slf4j
public class CartDownloadController {

    private final CartDownloadService cartDownloadService;
    private final JWTUtil jwtUtil;

    /**
     * 장바구니 항목 ID로 파일 다운로드
     */
    @GetMapping("/{cartItemId}")
    public ResponseEntity<byte[]> downloadCartItem(
            @PathVariable Long cartItemId,
            @RequestHeader("Authorization") String authHeader
    ) {
        // 토큰에서 사용자 ID 추출
        String token = authHeader.startsWith("Bearer ") ? authHeader.substring(7) : authHeader;
        String userId = jwtUtil.getUserid(token);

        log.info("장바구니 항목 다운로드 요청: cartItemId={}, userId={}", cartItemId, userId);
        return cartDownloadService.downloadCartItem(userId, cartItemId);
    }

    /**
     * 파일 ID로 직접 다운로드
     */
    @GetMapping("/file/{fileId}")
    public ResponseEntity<byte[]> downloadFile(
            @PathVariable Long fileId,
            @RequestHeader("Authorization") String authHeader
    ) {
        // 토큰에서 사용자 ID 추출
        String token = authHeader.startsWith("Bearer ") ? authHeader.substring(7) : authHeader;
        String userId = jwtUtil.getUserid(token);

        log.info("파일 직접 다운로드 요청: fileId={}, userId={}", fileId, userId);
        return cartDownloadService.downloadFileById(userId, fileId);
    }
}

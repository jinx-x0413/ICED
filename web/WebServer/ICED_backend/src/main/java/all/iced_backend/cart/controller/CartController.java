package all.iced_backend.cart.controller;

import all.iced_backend.auth.jwt.JWTUtil;
import all.iced_backend.cart.dto.AddToCartRequestDto;
import all.iced_backend.cart.dto.CartItemDto;
import all.iced_backend.cart.dto.CartResponseDto;
import all.iced_backend.cart.service.CartService;
import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/cart")
@CrossOrigin(origins = "*")
@RequiredArgsConstructor
public class CartController {

    private final CartService cartService;
    private final JWTUtil jwtUtil;

    /**
     * 장바구니에 파일 추가
     */
    @PostMapping("/add")
    public ResponseEntity<?> addToCart(
            @RequestBody AddToCartRequestDto requestDto,
            @RequestHeader("Authorization") String authHeader) {

        try {
            // Bearer 접두사 제거
            String token = authHeader;
            if (token.startsWith("Bearer ")) {
                token = token.substring(7);
            }

            // 토큰에서 사용자 ID 추출
            String userId = jwtUtil.getUserid(token);

            // 서비스 호출
            CartItemDto result = cartService.addToCart(userId, requestDto.getFileId());

            return ResponseEntity.ok(result);
        } catch (IllegalArgumentException e) {
            return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(e.getMessage());
        } catch (IllegalStateException e) {
            return ResponseEntity.status(HttpStatus.CONFLICT).body(e.getMessage());
        } catch (Exception e) {
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("장바구니 추가 중 오류 발생: " + e.getMessage());
        }
    }

    /**
     * 장바구니 조회
     */
    @GetMapping
    public ResponseEntity<?> getCart(@RequestHeader("Authorization") String authHeader) {
        try {
            // Bearer 접두사 제거
            String token = authHeader;
            if (token.startsWith("Bearer ")) {
                token = token.substring(7);
            }

            // 토큰에서 사용자 ID 추출
            String userId = jwtUtil.getUserid(token);

            // 서비스 호출
            CartResponseDto result = cartService.getCart(userId);

            return ResponseEntity.ok(result);
        } catch (Exception e) {
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("장바구니 조회 중 오류 발생: " + e.getMessage());
        }
    }

    /**
     * 장바구니에서 파일 제거
     */
    @DeleteMapping("/{fileId}")
    public ResponseEntity<?> removeFromCart(
            @PathVariable Long fileId,
            @RequestHeader("Authorization") String authHeader) {

        try {
            // Bearer 접두사 제거
            String token = authHeader;
            if (token.startsWith("Bearer ")) {
                token = token.substring(7);
            }

            // 토큰에서 사용자 ID 추출
            String userId = jwtUtil.getUserid(token);

            // 서비스 호출
            cartService.removeFromCart(userId, fileId);

            return ResponseEntity.ok("장바구니에서 항목이 제거되었습니다.");
        } catch (IllegalArgumentException e) {
            return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(e.getMessage());
        } catch (Exception e) {
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("장바구니 항목 제거 중 오류 발생: " + e.getMessage());
        }
    }

    /**
     * 장바구니 비우기
     */
    @DeleteMapping
    public ResponseEntity<?> clearCart(@RequestHeader("Authorization") String authHeader) {
        try {
            // Bearer 접두사 제거
            String token = authHeader;
            if (token.startsWith("Bearer ")) {
                token = token.substring(7);
            }

            // 토큰에서 사용자 ID 추출
            String userId = jwtUtil.getUserid(token);

            // 서비스 호출
            cartService.clearCart(userId);

            return ResponseEntity.ok("장바구니가 비워졌습니다.");
        } catch (Exception e) {
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("장바구니 비우기 중 오류 발생: " + e.getMessage());
        }
    }
}

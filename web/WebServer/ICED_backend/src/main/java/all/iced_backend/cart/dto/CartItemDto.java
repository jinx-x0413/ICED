package all.iced_backend.cart.dto;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class CartItemDto {
    private Long id;        // 장바구니 항목 ID
    private Long fileId;    // 파일 ID
    private String fileName;
    private String description;
    private String thumbnailUri;
    private Double size;
    private LocalDateTime addedAt;
}

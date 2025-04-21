package all.iced_backend.cart.service;

import all.iced_backend.cart.dto.CartItemDto;
import all.iced_backend.cart.dto.CartResponseDto;
import all.iced_backend.cart.entity.CartItem;
import all.iced_backend.cart.repository.CartRepository;
import all.iced_backend.list.entity.FileList;
import all.iced_backend.list.repository.FileListRepository;
import jakarta.transaction.Transactional;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Service
@RequiredArgsConstructor
public class CartService {

    private final CartRepository cartRepository;
    private final FileListRepository fileListRepository;

    /**
     * 장바구니에 파일 추가
     */
    @Transactional
    public CartItemDto addToCart(String userId, Long fileId) {
        // 이미 장바구니에 있는지 확인
        if (cartRepository.findByUserIdAndFileId(userId, fileId).isPresent()) {
            throw new IllegalStateException("이미 장바구니에 있는 파일입니다");
        }

        // 파일 존재 여부 확인
        FileList file = fileListRepository.findById(fileId)
                .orElseThrow(() -> new IllegalArgumentException("존재하지 않는 파일입니다: " + fileId));

        // 장바구니에 추가
        CartItem cartItem = new CartItem();
        cartItem.setUserId(userId);
        cartItem.setFileId(fileId);
        CartItem savedItem = cartRepository.save(cartItem);

        // 응답 생성
        return CartItemDto.builder()
                .id(savedItem.getId())
                .fileId(file.getId())
                .fileName(file.getFileName())
                .description(file.getDescription())
                .thumbnailUri(formatThumbnailUri(file.getThumbnailUri()))
                .size(file.getSize())
                .addedAt(savedItem.getAddedAt())
                .build();
    }

    /**
     * 사용자의 장바구니 조회
     */
    public CartResponseDto getCart(String userId) {
        // 사용자의 장바구니 항목 조회
        List<CartItem> cartItems = cartRepository.findByUserIdOrderByAddedAtDesc(userId);

        // 결과가 비어있으면 빈 응답 반환
        if (cartItems.isEmpty()) {
            return CartResponseDto.builder()
                    .userId(userId)
                    .items(new ArrayList<>())
                    .itemCount(0)
                    .build();
        }

        // 항목별 파일 정보 조회 및 DTO 변환
        List<CartItemDto> cartItemDtos = cartItems.stream()
                .map(item -> {
                    FileList file = fileListRepository.findById(item.getFileId()).orElse(null);
                    if (file == null) {
                        return null; // 파일이 없는 경우 null 반환 (나중에 필터링됨)
                    }

                    return CartItemDto.builder()
                            .id(item.getId())
                            .fileId(file.getId())
                            .fileName(file.getFileName())
                            .description(file.getDescription())
                            .thumbnailUri(formatThumbnailUri(file.getThumbnailUri()))
                            .size(file.getSize())
                            .addedAt(item.getAddedAt())
                            .build();
                })
                .filter(dto -> dto != null) // null 항목 제거
                .collect(Collectors.toList());

        // 응답 생성
        return CartResponseDto.builder()
                .userId(userId)
                .items(cartItemDtos)
                .itemCount(cartItemDtos.size())
                .build();
    }

    /**
     * 장바구니에서 항목 제거
     */
    @Transactional
    public void removeFromCart(String userId, Long fileId) {
        // 항목이 존재하는지 확인
        if (!cartRepository.findByUserIdAndFileId(userId, fileId).isPresent()) {
            throw new IllegalArgumentException("장바구니에 해당 항목이 없습니다");
        }

        // 항목 삭제
        cartRepository.deleteByUserIdAndFileId(userId, fileId);
    }

    /**
     * 장바구니 비우기
     */
    @Transactional
    public void clearCart(String userId) {
        cartRepository.deleteAllByUserId(userId);
    }

    /**
     * 썸네일 URI 포맷팅 (웹에서 접근 가능한 URL로 변환)
     */
    private String formatThumbnailUri(String thumbnailUri) {
        if (thumbnailUri == null) return null;

        // 파일명 추출
        int lastIndex = thumbnailUri.lastIndexOf('/');
        if (lastIndex >= 0) {
            String filename = thumbnailUri.substring(lastIndex + 1);
            return "/thumbnail/" + filename;
        }

        return thumbnailUri;
    }
}

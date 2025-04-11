package all.iced_backend.list.controller;

import all.iced_backend.list.service.ThumbnailService;
import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.context.request.async.DeferredResult;

import java.util.concurrent.TimeUnit;

@RestController
@CrossOrigin(origins = "*")
@RequiredArgsConstructor
public class ThumbnailController {

    private final ThumbnailService thumbnailService;

    @GetMapping("/thumbnail/{filename}")
    public DeferredResult<ResponseEntity<byte[]>> getThumbnail(@PathVariable String filename) {
        // DeferredResult 사용 - 타임아웃 10초 설정
        DeferredResult<ResponseEntity<byte[]>> deferredResult = new DeferredResult<>(10000L);

        // 비동기로 이미지 처리
        thumbnailService.getThumbnailImageAsync(filename)
                .thenAccept(imageData -> {
                    // 파일 확장자에 따른 미디어 타입 결정
                    String mediaType = "image/jpeg"; // 기본값
                    if (filename.toLowerCase().endsWith(".png")) {
                        mediaType = "image/png";
                    } else if (filename.toLowerCase().endsWith(".gif")) {
                        mediaType = "image/gif";
                    } else if (filename.toLowerCase().endsWith(".webp")) {
                        mediaType = "image/webp";
                    }

                    // 캐싱 설정과 함께 응답 생성
                    ResponseEntity<byte[]> responseEntity = ResponseEntity.ok()
                            .contentType(MediaType.parseMediaType(mediaType))
                            .header("Cache-Control", "max-age=86400") // 24시간 캐싱
                            .body(imageData);

                    deferredResult.setResult(responseEntity);
                })
                .exceptionally(ex -> {
                    deferredResult.setErrorResult(
                            ResponseEntity.status(HttpStatus.NOT_FOUND).body(null)
                    );
                    return null;
                });

        return deferredResult;
    }
}

package all.iced_backend.upload.controller;

import all.iced_backend.upload.dto.UploadResponseDto;
import all.iced_backend.upload.service.UploadService;
import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.util.concurrent.CompletableFuture;

@RestController
@RequestMapping("/api")
@CrossOrigin(origins = "*")
@RequiredArgsConstructor
public class UploadController {

    private final UploadService uploadService;

    @PostMapping("/upload")
    public CompletableFuture<ResponseEntity<UploadResponseDto>> uploadFile(
            @RequestParam("file") MultipartFile file,
            @RequestParam(value = "thumbnail", required = false) MultipartFile thumbnail,
            @RequestParam("uploader_id") String uploaderId,
            @RequestParam("description") String description,
            @RequestParam(value = "token", required = false) String token,
            @RequestHeader(value = "Authorization", required = false) String authHeader) {

        // 토큰 로깅 및 검증
        String finalToken = token;
        if (finalToken == null && authHeader != null) {
            if (authHeader.startsWith("Bearer ")) {
                finalToken = authHeader.substring(7);
            } else {
                finalToken = authHeader;
            }
        }

        System.out.println("Received token: " + finalToken);

        return uploadService.uploadFile(file, thumbnail, uploaderId, description)
                .thenApply(ResponseEntity::ok)
                .exceptionally(ex -> {
                    // 이곳에서 예외를 잡고 로그만 남기거나 기본 응답 리턴 가능
                    System.err.println("컨트롤러에서 비동기 예외 발생: " + (ex != null ? ex.getMessage() : "null"));
                    return ResponseEntity.status(500).body(null); // 또는 기본 메시지 포함
                });
    }
}

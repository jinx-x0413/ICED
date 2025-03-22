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
    public CompletableFuture<ResponseEntity<UploadResponseDto>> uploadFile(@RequestParam("file") MultipartFile file) {
        return uploadService.uploadFile(file)
                .thenApply(ResponseEntity::ok);
    }
}

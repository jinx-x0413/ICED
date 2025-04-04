package all.iced_backend.install.controller;

import org.springframework.core.io.Resource;
import org.springframework.core.io.ResourceLoader;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

import java.io.IOException;

@Controller
public class InstallController {

    private final ResourceLoader resourceLoader;

    public InstallController(ResourceLoader resourceLoader) {
        this.resourceLoader = resourceLoader;
    }

    @GetMapping("/download/installer")
    public ResponseEntity<Resource> downloadInstaller() throws IOException {
        // 리소스 경로 설정
        Resource resource = resourceLoader.getResource("classpath:static/iced-installer.exe");

        if (!resource.exists()) {
            throw new RuntimeException("File not found: iced-installer.exe");
        }

        return ResponseEntity.ok()
                .contentType(MediaType.APPLICATION_OCTET_STREAM)
                .header(HttpHeaders.CONTENT_DISPOSITION, "attachment; filename=\"iced-installer.exe\"")
                .body(resource);
    }
}

package all.iced_backend.list.service;

import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.concurrent.CompletableFuture;

@Service
@RequiredArgsConstructor
public class ThumbnailService {

    @Value("${ftps.host}")
    private String ftpsHost;

    @Value("${ftps.port}")
    private int ftpsPort;

    @Value("${ftps.username}")
    private String ftpsUser;

    @Value("${ftps.password}")
    private String ftpsPass;

    @Value("${ftps.remote-dir}")
    private String ftpsRemoteDir;

    private final ThumbnailFtpsDownloader ftpsDownloader = new ThumbnailFtpsDownloader();

    @Async("thumbnailTaskExecutor")
    public CompletableFuture<byte[]> getThumbnailImageAsync(String filename) {
        CompletableFuture<byte[]> future = new CompletableFuture<>();

        try {
            String thumbnailDir = ftpsRemoteDir + "/thumbnails";

            byte[] imageData = ftpsDownloader.downloadFileBytes(ftpsHost, ftpsPort, ftpsUser, ftpsPass, thumbnailDir, filename);
            future.complete(imageData);
        } catch (IOException e) {
            future.completeExceptionally(e);
        }

        return future;
    }
}

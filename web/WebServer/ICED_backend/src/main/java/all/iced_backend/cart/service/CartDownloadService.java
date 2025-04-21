package all.iced_backend.cart.service;

import all.iced_backend.cart.entity.CartItem;
import all.iced_backend.cart.repository.CartRepository;
import all.iced_backend.cart.util.CartFtpsDownloader;
import all.iced_backend.upload.entity.UploadFile;
import all.iced_backend.upload.repository.UploadFileRepository;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.NoSuchElementException;
import java.util.concurrent.CompletableFuture;

@Service
@RequiredArgsConstructor
@Slf4j
public class CartDownloadService {

    @Value("${ftps.host}")
    private String ftpsHost;

    @Value("${ftps.port}")
    private int ftpsPort;

    @Value("${ftps.username}")
    private String ftpsUser;

    @Value("${ftps.password}")
    private String ftpsPass;

    private final CartRepository cartRepository;
    private final UploadFileRepository uploadFileRepository;
    private final CartFtpsDownloader ftpsDownloader;

    /**
     * 장바구니 항목 ID로 파일 다운로드
     */
    public ResponseEntity<byte[]> downloadCartItem(String userId, Long cartItemId) {
        try {
            // 장바구니 항목 조회
            CartItem cartItem = cartRepository.findById(cartItemId)
                    .orElseThrow(() -> new NoSuchElementException("장바구니 항목을 찾을 수 없습니다. ID: " + cartItemId));

            // 권한 확인 (본인 장바구니만 다운로드 가능)
            if (!cartItem.getUserId().equals(userId)) {
                throw new SecurityException("다른 사용자의 장바구니 항목에 접근할 수 없습니다.");
            }

            // 파일 엔티티 조회
            UploadFile uploadFile = uploadFileRepository.findById(cartItem.getFileId())
                    .orElseThrow(() -> new NoSuchElementException("파일을 찾을 수 없습니다. ID: " + cartItem.getFileId()));

            log.info("장바구니 파일 다운로드 시도: fileId={}, fileUri={}", cartItem.getFileId(), uploadFile.getFileUri());

            // FTPS에서 파일 다운로드
            byte[] fileData = ftpsDownloader.downloadFile(
                    ftpsHost, ftpsPort, ftpsUser, ftpsPass, uploadFile.getFileUri());

            // 파일 이름 확인 및 안전하게 처리 (description 사용)
            String filename = getFilenameFromDescription(uploadFile);
            log.info("다운로드 파일명: {}", filename);

            // 응답 헤더 설정 (개선된 방식)
            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
            setContentDispositionHeader(headers, filename);
            headers.setContentLength(fileData.length);

            return new ResponseEntity<>(fileData, headers, HttpStatus.OK);

        } catch (NoSuchElementException e) {
            log.error("리소스 찾기 실패", e);
            throw new RuntimeException("리소스를 찾을 수 없습니다: " + e.getMessage(), e);
        } catch (SecurityException e) {
            log.error("권한 오류", e);
            throw new RuntimeException("권한이 없습니다: " + e.getMessage(), e);
        } catch (IOException e) {
            log.error("파일 다운로드 실패", e);
            throw new RuntimeException("파일 다운로드 중 오류가 발생했습니다: " + e.getMessage(), e);
        }
    }

    /**
     * 파일 ID로 직접 다운로드
     */
    public ResponseEntity<byte[]> downloadFileById(String userId, Long fileId) {
        try {
            // 파일 엔티티 조회
            UploadFile uploadFile = uploadFileRepository.findById(fileId)
                    .orElseThrow(() -> new NoSuchElementException("파일을 찾을 수 없습니다. ID: " + fileId));

            log.info("파일 직접 다운로드 시도: fileId={}, fileUri={}", fileId, uploadFile.getFileUri());

            // FTPS에서 파일 다운로드
            byte[] fileData = ftpsDownloader.downloadFile(
                    ftpsHost, ftpsPort, ftpsUser, ftpsPass, uploadFile.getFileUri());

            // 파일 이름 확인 및 안전하게 처리 (description 사용)
            String filename = getFilenameFromDescription(uploadFile);
            log.info("다운로드 파일명: {}", filename);

            // 응답 헤더 설정 (개선된 방식)
            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
            setContentDispositionHeader(headers, filename);
            headers.setContentLength(fileData.length);

            return new ResponseEntity<>(fileData, headers, HttpStatus.OK);

        } catch (NoSuchElementException e) {
            log.error("리소스 찾기 실패", e);
            throw new RuntimeException("리소스를 찾을 수 없습니다: " + e.getMessage(), e);
        } catch (IOException e) {
            log.error("파일 다운로드 실패", e);
            throw new RuntimeException("파일 다운로드 중 오류가 발생했습니다: " + e.getMessage(), e);
        }
    }

    /**
     * 비동기 방식으로 파일 다운로드 (대용량 파일용)
     */
    @Async("downloadTaskExecutor")
    public CompletableFuture<byte[]> downloadFileAsync(String fileUri) {
        CompletableFuture<byte[]> future = new CompletableFuture<>();

        try {
            byte[] fileData = ftpsDownloader.downloadFile(
                    ftpsHost, ftpsPort, ftpsUser, ftpsPass, fileUri);
            future.complete(fileData);
        } catch (IOException e) {
            future.completeExceptionally(e);
        }

        return future;
    }

    /**
     * description 필드를 사용하여 파일명 생성
     * 확장자가 없으면 .glb 추가
     */
    private String getFilenameFromDescription(UploadFile uploadFile) {
        // 우선 description 필드 사용
        String description = uploadFile.getDescription();

        if (description != null && !description.trim().isEmpty()) {
            // description에 확장자가 없으면 .glb 추가
            if (!description.toLowerCase().endsWith(".glb")) {
                return description + ".glb";
            }
            return description;
        }

        // description이 없는 경우 대체 로직
        String filename = uploadFile.getFileName();

        // fileName도 없는 경우 URI에서 추출
        if (filename == null || filename.trim().isEmpty()) {
            String fileUri = uploadFile.getFileUri();
            int lastSlashIndex = fileUri.lastIndexOf('/');

            if (lastSlashIndex != -1 && lastSlashIndex < fileUri.length() - 1) {
                filename = fileUri.substring(lastSlashIndex + 1);
            } else {
                // 기본 이름과 확장자 사용
                filename = "model.glb";
            }
        }

        // 확장자가 없으면 .glb 추가
        if (!filename.contains(".")) {
            filename += ".glb";
        }

        return filename;
    }

    /**
     * Content-Disposition 헤더 설정
     * RFC 6266 규격 준수
     */
    private void setContentDispositionHeader(HttpHeaders headers, String filename) {
        try {
            // 특수문자 포함된 파일명을 위한 URL 인코딩
            String encodedFilename = URLEncoder.encode(filename, StandardCharsets.UTF_8.toString())
                    .replace("+", "%20");  // 공백 문자 처리

            // RFC 6266 규격에 맞는 헤더 설정 (ASCII 및 UTF-8 형식 모두 제공)
            headers.add(HttpHeaders.CONTENT_DISPOSITION,
                    "attachment; filename=\"" + filename + "\"; filename*=UTF-8''" + encodedFilename);
        } catch (Exception e) {
            log.warn("파일명 인코딩 실패, 기본 방식으로 대체: {}", e.getMessage());
            headers.setContentDispositionFormData("attachment", filename);
        }
    }
}

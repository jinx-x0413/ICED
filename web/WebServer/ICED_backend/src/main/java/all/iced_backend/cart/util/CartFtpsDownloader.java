package all.iced_backend.cart.util;

import org.apache.commons.net.ftp.FTPReply;
import org.apache.commons.net.ftp.FTPSClient;
import org.springframework.stereotype.Component;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

@Component
public class CartFtpsDownloader {

    /**
     * FTPS 서버에서 파일 다운로드
     * @param fileUri DB에 저장된 파일 경로 (/uploads/models/파일명.확장자 형식)
     */
    public byte[] downloadFile(String server, int port, String user, String pass, String fileUri) throws IOException {
        // Explicit FTPS 연결 (TLS)
        FTPSClient ftpsClient = new FTPSClient("TLS", false);
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

        try {
            // 서버 연결
            ftpsClient.connect(server, port);
            int replyCode = ftpsClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(replyCode)) {
                throw new IOException("FTPS 서버 연결 실패. 응답 코드: " + replyCode);
            }

            // 로그인
            if (!ftpsClient.login(user, pass)) {
                throw new IOException("FTPS 로그인 실패");
            }

            // FTPS 보안 설정
            ftpsClient.execPBSZ(0);
            ftpsClient.execPROT("P");
            ftpsClient.enterLocalPassiveMode();
            ftpsClient.setFileType(FTPSClient.BINARY_FILE_TYPE);

            // 파일 경로에서 디렉토리와 파일명 분리
            String directory = getDirectoryFromUri(fileUri);
            String filename = getFilenameFromUri(fileUri);

            // 디렉토리 이동
            if (!ftpsClient.changeWorkingDirectory(directory)) {
                throw new IOException("원격 디렉토리 이동 실패: " + directory);
            }

            // 파일 다운로드
            boolean success = ftpsClient.retrieveFile(filename, outputStream);
            if (!success) {
                throw new IOException("파일 다운로드 실패: " + filename);
            }

            return outputStream.toByteArray();

        } finally {
            try {
                if (ftpsClient.isConnected()) {
                    ftpsClient.logout();
                    ftpsClient.disconnect();
                }
            } catch (IOException ex) {
                // 연결 종료 오류는 무시
            }
            outputStream.close();
        }
    }

    /**
     * 파일 URI에서 디렉토리 경로 추출
     */
    private String getDirectoryFromUri(String fileUri) {
        // 앞의 슬래시(/) 제거
        if (fileUri.startsWith("/")) {
            fileUri = fileUri.substring(1);
        }

        int lastSlashIndex = fileUri.lastIndexOf('/');
        if (lastSlashIndex != -1) {
            return fileUri.substring(0, lastSlashIndex);
        }
        return ""; // 디렉토리가 없는 경우 루트 디렉토리 사용
    }

    /**
     * 파일 URI에서 파일명 추출
     */
    private String getFilenameFromUri(String fileUri) {
        int lastSlashIndex = fileUri.lastIndexOf('/');
        if (lastSlashIndex != -1 && lastSlashIndex < fileUri.length() - 1) {
            return fileUri.substring(lastSlashIndex + 1);
        }
        return fileUri; // 슬래시가 없는 경우 전체 경로를 파일명으로 사용
    }
}

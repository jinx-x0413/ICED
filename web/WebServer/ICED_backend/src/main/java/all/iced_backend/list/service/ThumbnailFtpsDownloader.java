package all.iced_backend.list.service;


import org.apache.commons.net.ftp.FTPReply;
import org.apache.commons.net.ftp.FTPSClient;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * FTPS 서버에서 썸네일 이미지를 다운로드하는 유틸리티 클래스
 */
public class ThumbnailFtpsDownloader {

    public byte[] downloadFileBytes(String server, int port, String user, String pass,
                                    String remoteDir, String filename) throws IOException {
        // Explicit FTPS 연결 (TLS)
        FTPSClient ftpsClient = new FTPSClient("TLS", false);
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

        try {
            ftpsClient.connect(server, port);
            int replyCode = ftpsClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(replyCode)) {
                throw new IOException("FTPS 서버 연결 실패. 응답 코드: " + replyCode);
            }

            if (!ftpsClient.login(user, pass)) {
                throw new IOException("FTPS 로그인 실패");
            }

            // FTPS 보안 설정
            ftpsClient.execPBSZ(0);
            ftpsClient.execPROT("P");
            ftpsClient.enterLocalPassiveMode();
            ftpsClient.setFileType(FTPSClient.BINARY_FILE_TYPE);

            // 디렉토리 이동
            if (!ftpsClient.changeWorkingDirectory(remoteDir)) {
                throw new IOException("원격 디렉토리 이동 실패: " + remoteDir);
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
                ex.printStackTrace();
            }
            outputStream.close();
        }
    }
}

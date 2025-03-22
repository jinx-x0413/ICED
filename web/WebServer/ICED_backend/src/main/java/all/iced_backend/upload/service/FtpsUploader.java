package all.iced_backend.upload.service;

import org.apache.commons.net.ftp.FTPSClient;
import org.apache.commons.net.ftp.FTPReply;
import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * FTPS를 통해 파일을 업로드하는 유틸리티 클래스
 * Apache Commons Net 라이브러리 사용 (implementation 'commons-net:commons-net:3.9.0')
 */
public class FtpsUploader {

    /**
     * FTPS 서버로 파일 업로드
     * @param server      FTPS 서버 주소
     * @param port        FTPS 서버 포트 (일반적으로 990)
     * @param user        로그인 계정
     * @param pass        로그인 비밀번호
     * @param localFile   업로드할 로컬(임시) 파일
     * @param remoteDir   FTPS 서버 상의 디렉토리 경로 (예: "/uploads")
     * @return 업로드된 파일의 전체 경로 (예: "/uploads/filename") or null (실패 시)
     */
    public String uploadFile(String server, int port, String user, String pass,
                             File localFile, String remoteDir) {
        // Explicit FTPS (TLS)
        FTPSClient ftpsClient = new FTPSClient("TLS", false);

        try {
            ftpsClient.connect(server, port);
            int replyCode = ftpsClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(replyCode)) {
                System.out.println("FTPS 서버 연결 실패. 응답 코드: " + replyCode);
                return null;
            }

            if (!ftpsClient.login(user, pass)) {
                System.out.println("FTPS 로그인 실패");
                return null;
            }

            // 패시브 모드
            ftpsClient.enterLocalPassiveMode();
            // 바이너리 전송
            ftpsClient.setFileType(FTPSClient.BINARY_FILE_TYPE);

            // 원격 디렉토리 이동
            ftpsClient.changeWorkingDirectory(remoteDir);

            // 파일 업로드
            String remoteFileName = localFile.getName();
            try (InputStream inputStream = new FileInputStream(localFile)) {
                boolean done = ftpsClient.storeFile(remoteFileName, inputStream);
                if (done) {
                    System.out.println("FTPS 업로드 성공: " + remoteFileName);
                    // 예: "/uploads/filename.ext"
                    return remoteDir.endsWith("/")
                            ? remoteDir + remoteFileName
                            : remoteDir + "/" + remoteFileName;
                } else {
                    System.out.println("FTPS 업로드 실패");
                    return null;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                if (ftpsClient.isConnected()) {
                    ftpsClient.logout();
                    ftpsClient.disconnect();
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }
}

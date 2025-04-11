package all.iced_backend.list.service;

import org.apache.commons.net.ftp.FTPReply;
import org.apache.commons.net.ftp.FTPSClient;

import java.io.IOException;

/**
 * FTPS 서버에서 파일 삭제를 처리하는 클래스
 */
public class FtpsFileManager {

    /**
     * FTPS 서버에서 파일 삭제
     * @param server      FTPS 서버 주소
     * @param port        FTPS 서버 포트
     * @param user        로그인 계정
     * @param pass        로그인 비밀번호
     * @param remoteFilePath   삭제할 파일의 전체 경로 (예: "/uploads/models/filename.obj")
     * @return 삭제 성공 여부
     */
    public boolean deleteFile(String server, int port, String user, String pass,
                              String remoteFilePath) {
        FTPSClient ftpsClient = new FTPSClient("TLS", false);

        try {
            ftpsClient.connect(server, port);
            int replyCode = ftpsClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(replyCode)) {
                System.out.println("FTPS 서버 연결 실패. 응답 코드: " + replyCode);
                return false;
            }

            if (!ftpsClient.login(user, pass)) {
                System.out.println("FTPS 로그인 실패");
                return false;
            }

            // FTPS 보안 설정
            ftpsClient.execPBSZ(0);
            ftpsClient.execPROT("P");
            ftpsClient.enterLocalPassiveMode();

            // 파일 삭제
            boolean deleted = ftpsClient.deleteFile(remoteFilePath);
            if (deleted) {
                System.out.println("✅ FTPS 파일 삭제 성공: " + remoteFilePath);
            } else {
                System.out.println("❌ FTPS 파일 삭제 실패: " + remoteFilePath);
                System.out.println("응답 코드: " + ftpsClient.getReplyCode());
                System.out.println("응답 메시지: " + ftpsClient.getReplyString());
            }
            return deleted;

        } catch (IOException e) {
            e.printStackTrace();
            return false;
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

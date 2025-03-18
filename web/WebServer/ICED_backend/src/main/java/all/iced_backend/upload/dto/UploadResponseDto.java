package all.iced_backend.upload.dto;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class UploadResponseDto {
    private Long id;          // DB에 저장된 ID
    private String fileName;  // 원본 파일명
    private String fileUri;   // FTPS 서버 경로
}

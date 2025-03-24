package all.iced_backend.upload.dto;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@AllArgsConstructor
public class UploadResponseDto {
    private Long id;          // DB에 저장된 ID
    private String fileName;  // 원본 파일명
    private String fileUri;   // FTPS 서버 경로
    private String uploader_id; // FTPS 서버에 저장된 경로 (업로드한 userid)
    private String description; //파일 설명
    private Double size; //파일 크기

}

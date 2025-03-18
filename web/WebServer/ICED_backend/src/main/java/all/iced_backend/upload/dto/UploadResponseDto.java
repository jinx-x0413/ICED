package all.iced_backend.upload.dto;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class UploadResponseDto {
    private Long id;
    private String fileName;
    private String fileUri;
}

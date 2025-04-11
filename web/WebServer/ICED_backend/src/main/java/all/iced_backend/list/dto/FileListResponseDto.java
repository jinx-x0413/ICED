package all.iced_backend.list.dto;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Builder
public class FileListResponseDto {
    private Long id;
    private String fileName;
    private String fileUri;
    private String uploader_id;
    private String description;
    private Double size;
    private String thumbnailUri;
    private String createdAt;
}

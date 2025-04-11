package all.iced_backend.list.dto;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Builder
public class FileUpdateResponseDto {
    private Long id;
    private String fileName;
    private String fileUri;
    private String thumbnailUri;
    private String description;
    private Double size;
    private LocalDateTime createdAt;
}

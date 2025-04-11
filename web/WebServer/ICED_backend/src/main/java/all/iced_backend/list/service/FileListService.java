package all.iced_backend.list.service;

import all.iced_backend.list.dto.FileListResponseDto;
import all.iced_backend.list.entity.FileList;
import all.iced_backend.list.repository.FileListRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.stream.Collectors;

@Service
@RequiredArgsConstructor
public class FileListService {

    private final FileListRepository fileListRepository;

    /**
     * 모든 파일 목록을 최신순으로 조회
     */
    public List<FileListResponseDto> getAllFiles() {
        List<FileList> files = fileListRepository.findAllOrderByCreatedAtDesc();
        return files.stream()
                .map(this::convertToDto)
                .collect(Collectors.toList());
    }

    /**
     * 특정 사용자의 파일 목록 조회
     */
    public List<FileListResponseDto> getFilesByUploader(String uploaderId) {
        List<FileList> files = fileListRepository.findByUploaderId(uploaderId);
        return files.stream()
                .map(this::convertToDto)
                .collect(Collectors.toList());
    }

    /**
     * 파일 ID로 단일 파일 조회
     */
    public FileListResponseDto getFileById(Long id) {
        FileList file = fileListRepository.findById(id)
                .orElseThrow(() -> new RuntimeException("File not found with id: " + id));
        return convertToDto(file);
    }

    /**
     * Entity를 DTO로 변환
     */
    private FileListResponseDto convertToDto(FileList file) {
        // 썸네일 URI가 있을 경우 웹에서 접근 가능한 URL로 변환
        String thumbnailUrl = null;
        if (file.getThumbnailUri() != null) {
            String filename = extractFilename(file.getThumbnailUri());
            thumbnailUrl = "/thumbnail/" + filename;
        }

        return FileListResponseDto.builder()
                .id(file.getId())
                .fileName(file.getFileName())
                .fileUri(file.getFileUri())
                .uploader_id(file.getUploader_id())
                .description(file.getDescription())
                .size(file.getSize())
                .createdAt(file.getCreatedAt())
                .thumbnailUri(thumbnailUrl)
                .build();
    }

    /**
     * 경로에서 파일명만 추출
     */
    private String extractFilename(String fullPath) {
        if (fullPath == null) return null;
        int lastIndex = fullPath.lastIndexOf('/');
        if (lastIndex >= 0) {
            return fullPath.substring(lastIndex + 1);
        }
        return fullPath;
    }
}

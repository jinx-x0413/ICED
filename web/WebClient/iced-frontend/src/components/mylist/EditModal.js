// components/mylist/EditModal.js
import React, { useState, useEffect } from 'react';

const EditModal = ({ isOpen, onClose, file, onUpdate }) => {
    const [description, setDescription] = useState('');
    const [newFile, setNewFile] = useState(null);
    const [newThumbnail, setNewThumbnail] = useState(null);
    const [isSubmitting, setIsSubmitting] = useState(false);
    const [thumbnailPreview, setThumbnailPreview] = useState(null);
    const [error, setError] = useState(''); // 오류 메시지 상태 추가

    // 모달이 열릴 때마다 초기값 설정
    useEffect(() => {
        if (isOpen && file) {
            setDescription(file.description || '');
            setNewFile(null);
            setNewThumbnail(null);
            setError(''); // 오류 초기화
            setThumbnailPreview(
                file.thumbnailUri 
                    ? (file.thumbnailUri.startsWith('http') 
                        ? file.thumbnailUri 
                        : `http://localhost:8080${file.thumbnailUri}`)
                    : null
            );
        }
    }, [isOpen, file]);

    // 모달이 닫힐 때 상태 초기화
    const handleClose = () => {
        setDescription('');
        setNewFile(null);
        setNewThumbnail(null);
        setThumbnailPreview(null);
        setError('');
        onClose();
    };

    // 모달이 닫혀있을 때는 렌더링하지 않음
    if (!isOpen) return null;
    
    // 모델 파일 유효성 검사
    const validateModelFile = (file) => {
        const allowedExtensions = ['.gltf', '.glb'];
        const fileExtension = file.name.slice(file.name.lastIndexOf(".")).toLowerCase();
        return allowedExtensions.includes(fileExtension);
    };
    
    // 썸네일 파일 유효성 검사
    const validateThumbnail = (file) => {
        const allowedTypes = ['image/jpeg', 'image/png', 'image/gif']; // 허용된 이미지 MIME 타입
        return allowedTypes.includes(file.type);
    };
    
    // 모델 파일 변경 처리
    const handleFileChange = (e) => {
        const selectedFile = e.target.files[0];
        if (selectedFile) {
            if (!validateModelFile(selectedFile)) {
                setError("올바른 파일 형식이 아닙니다. .gltf 또는 .glb 파일만 업로드 가능합니다.");
                e.target.value = ''; // 파일 입력 초기화
                return;
            }
            setNewFile(selectedFile);
            setError(''); // 오류 초기화
        }
    };
    
    // 썸네일 변경 처리
    const handleThumbnailChange = (e) => {
        const selectedFile = e.target.files[0];
        if (selectedFile) {
            if (!validateThumbnail(selectedFile)) {
                setError("올바른 이미지 파일 형식이 아닙니다. (.jpg, .png, .gif만 허용)");
                e.target.value = ''; // 파일 입력 초기화
                return;
            }

            setNewThumbnail(selectedFile);
            
            // 미리보기 URL 생성
            const previewUrl = URL.createObjectURL(selectedFile);
            setThumbnailPreview(previewUrl);
            setError(''); // 오류 초기화
        }
    };

    // 폼 제출 처리
    const handleSubmit = async (e) => {
        e.preventDefault();
        
        if (!description.trim()) {
            setError("파일 설명을 입력해주세요.");
            return;
        }
        
        setIsSubmitting(true);
        setError(''); // 오류 초기화
        
        const formData = new FormData();
        formData.append('description', description);
        
        if (newFile) {
            formData.append('file', newFile);
        }
        
        if (newThumbnail) {
            formData.append('thumbnail', newThumbnail);
        }
        
        try {
            await onUpdate(file.id, formData);
            handleClose();
        } catch (error) {
            console.error('파일 업데이트 중 오류:', error);
            setError(error.message || '파일 업데이트에 실패했습니다.');
        } finally {
            setIsSubmitting(false);
        }
    };

    // 파일 이름 표시 함수
    const getFileName = (file) => {
        if (!file) return '';
        return file.name.length > 20 ? `${file.name.substring(0, 20)}...` : file.name;
    };

    return (
        <div className="mylist-modal-overlay">
            <div className="mylist-modal-content">
                <div className="mylist-modal-header">
                    <h2>파일 정보 수정</h2>
                    <button className="mylist-close-button" onClick={handleClose}>&times;</button>
                </div>
                
                <form className="mylist-form" onSubmit={handleSubmit}>
                    {error && (
                        <div className="mylist-error-message">
                            <span className="mylist-error-icon">⚠️</span>
                            {error}
                        </div>
                    )}
                    
                    <div className="mylist-form-group">
                        <label htmlFor="edit-description">파일 설명:</label>
                        <textarea 
                            id="edit-description" 
                            value={description}
                            onChange={(e) => setDescription(e.target.value)}
                            rows="3"
                            placeholder="파일에 대한 설명을 입력하세요"
                            className="mylist-textarea"
                        />
                    </div>
                    
                    <div className="mylist-form-row">
                        <div className="mylist-form-group">
                            <label>모델 파일 교체 (선택사항):</label>
                            <div className="mylist-file-input-container">
                                <input 
                                    type="file" 
                                    id="edit-file"
                                    onChange={handleFileChange}
                                    className="mylist-file-input-hidden"
                                    accept=".gltf, .glb"
                                />
                                <label htmlFor="edit-file" className="mylist-custom-file-button">
                                    파일 선택
                                </label>
                                <span className="mylist-file-name">
                                    {newFile ? getFileName(newFile) : '선택된 파일 없음'}
                                </span>
                            </div>
                            {newFile && (
                                <div className="mylist-file-info-preview">
                                    <span>파일 크기: {(newFile.size / (1024 * 1024)).toFixed(2)} MB</span>
                                </div>
                            )}
                        </div>
                        
                        <div className="mylist-form-group">
                            <label>썸네일 교체 (선택사항):</label>
                            <div className="mylist-file-input-container">
                                <input 
                                    type="file" 
                                    id="edit-thumbnail"
                                    accept="image/jpeg, image/png, image/gif"
                                    onChange={handleThumbnailChange}
                                    className="mylist-file-input-hidden"
                                />
                                <label htmlFor="edit-thumbnail" className="mylist-custom-file-button">
                                    이미지 선택
                                </label>
                                <span className="mylist-file-name">
                                    {newThumbnail ? getFileName(newThumbnail) : '선택된 이미지 없음'}
                                </span>
                            </div>
                            
                            {thumbnailPreview && (
                                <div className="mylist-thumbnail-preview">
                                    <img src={thumbnailPreview} alt="Thumbnail preview" />
                                </div>
                            )}
                        </div>
                    </div>
                    
                    <div className="mylist-form-actions">
                        <button 
                            type="button" 
                            className="mylist-action-button mylist-cancel"
                            onClick={handleClose}
                            disabled={isSubmitting}
                        >
                            취소
                        </button>
                        <button 
                            type="submit" 
                            className="mylist-action-button mylist-submit"
                            disabled={isSubmitting}
                        >
                            {isSubmitting ? '처리 중...' : '수정 완료'}
                        </button>
                    </div>
                </form>
            </div>
        </div>
    );
};

export default EditModal;

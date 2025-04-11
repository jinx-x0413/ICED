// components/DetailModal.js
import React from 'react';

const DetailModal = ({ isOpen, onClose, file }) => {
    if (!isOpen) return null;

    // 날짜 포맷 함수
    const formatDate = (dateString) => {
        if (!dateString) return '날짜 정보 없음';
        
        const date = new Date(dateString);
        return date.toLocaleString('ko-KR', {
            year: 'numeric',
            month: 'long',
            day: 'numeric',
            hour: '2-digit',
            minute: '2-digit'
        });
    };
    
    // 파일 크기 포맷
    const formatFileSize = (size) => {
        if (!size) return '크기 정보 없음';
        return `${size.toFixed(2)} MB`;
    };

    return (
        <div className="mylist-modal-overlay">
            <div className="mylist-modal-content mylist-detail-modal">
                <div className="mylist-modal-header">
                    <h2>파일 상세정보</h2>
                    <button className="mylist-close-button" onClick={onClose}>&times;</button>
                </div>
                
                <div className="mylist-detail-content">
                    <div className="mylist-detail-image">
                        {file.thumbnailUri ? (
                            <img 
                                src={file.thumbnailUri.startsWith('http') 
                                    ? file.thumbnailUri 
                                    : `http://localhost:8080${file.thumbnailUri}`
                                } 
                                alt={file.description || 'File thumbnail'} 
                            />
                        ) : (
                            <div className="mylist-no-thumbnail-large">
                                <span>No Image</span>
                            </div>
                        )}
                    </div>
                    
                    <div className="mylist-detail-info">
                        <h3>{file.description || '제목 없음'}</h3>
                        
                        <table className="mylist-detail-table">
                            <tbody>
                                <tr>
                                    <th>업로드 ID</th>
                                    <td>{file.uploader_id || '설명 없음'}</td>
                                </tr>
                                <tr>
                                    <th>파일 크기</th>
                                    <td>{formatFileSize(file.size)}</td>
                                </tr>
                                <tr>
                                    <th>업로드 날짜</th>
                                    <td>{formatDate(file.createdAt)}</td>
                                </tr>
                            </tbody>
                        </table>
                        
                        <div className="mylist-detail-actions">
                            <button 
                                className="mylist-action-button mylist-edit mylist-large" 
                                onClick={onClose}
                            >
                                닫기
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default DetailModal;

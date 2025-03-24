// Upload.js
import React, { useState, useRef } from 'react';
import { useAuth } from '../AuthContext';
import './css/upload.css';

const Upload = () => {
    const { state } = useAuth();
    const user = state.user;
    const [description, setDescription] = useState('');
    const [progress, setProgress] = useState(0);
    const [status, setStatus] = useState('');
    const [statusType, setStatusType] = useState(''); // 'success', 'error', 'loading'
    const [fileName, setFileName] = useState('');
    const fileInputRef = useRef(null);

    const handleFileSelect = () => {
        // 숨겨진 파일 입력 클릭
        fileInputRef.current.click();
    };

    const handleFileChange = (e) => {
        if (e.target.files && e.target.files[0]) {
            setFileName(e.target.files[0].name);
        }
    };

    const handleFileUpload = async (e) => {
        e.preventDefault();

        if (!window.confirm("업로드하시겠습니까?")) {
            return;
        }

        const fileInput = fileInputRef.current;
        const file = fileInput.files[0];

        if (!file) {
            setStatus("파일을 선택해주세요.");
            setStatusType('error');
            return;
        }

        const formData = new FormData();
        formData.append("file", file);
        formData.append("uploader_id", user.userid);
        formData.append("description", description);

        setProgress(0);
        setStatus("업로드 중...");
        setStatusType('loading');

        const xhr = new XMLHttpRequest();
        xhr.open("POST", "http://localhost:8080/api/upload", true);

        xhr.upload.onprogress = function(event) {
            if (event.lengthComputable) {
                const percent = (event.loaded / event.total) * 100;
                setProgress(percent);
            }
        };

        xhr.onload = function() {
            if (xhr.status === 200) {
                setStatus("업로드 성공!");
                setStatusType('success');
                // 폼 초기화
                setDescription('');
                setFileName('');
                fileInput.value = '';
            } else {
                setStatus("업로드 실패. 다시 시도해주세요.");
                setStatusType('error');
            }
        };

        xhr.onerror = function() {
            setStatus("네트워크 오류가 발생했습니다.");
            setStatusType('error');
        };

        xhr.send(formData);
    };

    return (
        <div className="upload-container">
            <div className="upload-card">
                <h1 className="upload-title">파일 업로드</h1>
                <form className="upload-form" onSubmit={handleFileUpload}>
                    <div className="file-input-container">
                        <label className="file-input-label"></label>
                        
                        {/* 숨겨진 파일 입력 */}
                        <input 
                            type="file" 
                            ref={fileInputRef}
                            onChange={handleFileChange}
                            className="hidden-file-input"
                        />
                        
                        {/* 커스텀 파일 선택 버튼 */}
                        <div className="file-select-button" onClick={handleFileSelect}>
                            파일 업로드
                        </div>
                        
                        {/* 선택된 파일 이름 표시 */}
                        {fileName && (
                            <div className="file-name">
                                {fileName}
                            </div>
                        )}
                    </div>
                    
                    <input 
                        type="text" 
                        className="description-input"
                        value={description}
                        onChange={(e) => setDescription(e.target.value)}
                        placeholder="파일 설명을 입력해주세요"
                    />
                    
                    <button 
                        type="submit" 
                        className="upload-button" 
                        disabled={!description || !fileName}
                    >
                        업로드
                    </button>
                </form>

                <div className="progress-container">
                    <div className="progress">
                        <div className="progress-bar" style={{ width: `${progress}%` }}></div>
                    </div>
                    {status && (
                        <div className={`status ${statusType}`}>
                            {status}
                        </div>
                    )}
                </div>
            </div>
        </div>
    );
};

export default Upload;

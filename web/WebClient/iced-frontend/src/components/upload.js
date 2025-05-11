// Upload.js
import React, { useState, useRef } from 'react';
import { useAuth } from '../AuthContext';
import './css/upload.css';

const baseURL = process.env.REACT_APP_BASE_URL;

const Upload = () => {
    const { state } = useAuth();
    const user = state.user;
    const [description, setDescription] = useState('');
    const [progress, setProgress] = useState(0);
    const [status, setStatus] = useState('');
    const [statusType, setStatusType] = useState(''); // 'success', 'error', 'loading'
    const [fileName, setFileName] = useState('');
    const [thumbnail, setThumbnail] = useState(null); // 썸네일 상태 추가
    const fileInputRef = useRef(null);
    const thumbnailInputRef = useRef(null); // 썸네일 파일 입력 참조 추가

    const handleFileSelect = () => {
        // 숨겨진 파일 입력 클릭
        fileInputRef.current.click();
    };

    const handleFileChange = (e) => {
        if (e.target.files && e.target.files[0]) {
            const file = e.target.files[0];
            const allowedExtensions = ['.gltf', '.glb'];
            const fileExtension = file.name.slice(file.name.lastIndexOf(".")).toLowerCase(); 
            if (!allowedExtensions.includes(fileExtension)) {
                setStatus("올바른 파일 형식이 아닙니다. .gltf 또는 .glb 파일만 업로드 가능합니다.");
                setStatusType('error');
                setFileName(''); // 파일 이름 초기화
                e.target.value = ''; // 파일 입력 초기화
                return;
            }
            setFileName(e.target.files[0].name);
            setStatus(''); // 상태 초기화
            setStatusType('');
        }
    };

    const handleThumbnailSelect = () => {
        // 숨겨진 썸네일 입력 클릭 
         thumbnailInputRef.current.click();
    };

    const handleThumbnailChange = (e) => {
        if (e.target.files && e.target.files[0]) {
            const file = e.target.files[0];
            const allowedTypes = ['image/jpeg', 'image/png', 'image/gif']; // 허용된 이미지 MIME 타입
            if (!allowedTypes.includes(file.type)) {
                setStatus("올바른 이미지 파일 형식이 아닙니다. (.jpg, .png, .gif만 허용)");
                setStatusType('error');
                e.target.value = ''; // 파일 입력 초기화
                return;
            }

            const reader = new FileReader();
            reader.onload = () => {
                setThumbnail(reader.result); // 썸네일 미리보기 URL 설정
            };
            reader.readAsDataURL(file);
            setStatus(''); // 상태 초기화
            setStatusType('');
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
        
        // 썸네일 파일 추가
        const thumbnailFile = thumbnailInputRef.current.files[0];
        if (thumbnailFile) {
            formData.append("thumbnail", thumbnailFile);
        }

        setProgress(0);
        setStatus("업로드 중...");
        setStatusType('loading');
    
        const xhr = new XMLHttpRequest();
        xhr.open("POST", `${baseURL}/api/upload`, true);
    
        // LocalStorage에서 토큰 가져오기
        const token = localStorage.getItem("accessToken");
        xhr.setRequestHeader('ngrok-skip-browser-warning', 'true');

        console.log("accessToken:", token);


        // 요청 헤더에 Authorization과 credentials 추가
        if (token) {
            if (token.startsWith('Bearer ')) {
                xhr.setRequestHeader('Authorization', token);
            } else {
                xhr.setRequestHeader('Authorization', `Bearer ${token}`);
            }
        }
        xhr.withCredentials = true; // credentials: "include"와 동일하게 작동
    
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
                setThumbnail(null); // 썸네일 초기화
                fileInput.value = '';
            } else {
                console.error("서버 응답:", xhr.status, xhr.statusText, xhr.responseText);
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
                            accept=".gltf, .glb"
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
                        
                    {/* 썸네일 업로드 */}
                    <div className="thumbnail-input-container">
                        
                        {/* 숨겨진 썸네일 파일 입력 */}
                        <input 
                            type="file" 
                            ref={thumbnailInputRef}
                            onChange={handleThumbnailChange}
                            className="hidden-thumbnail-input"
                            accept=".jpeg, .png, .gif" // 이미지 파일만 허용
                        />
                        
                        {/* 커스텀 썸네일 선택 버튼 */}
                        <div className="thumbnail-select-button" onClick={handleThumbnailSelect}>
                            썸네일 선택
                        </div>
                        
                        {/* 썸네일 미리보기 */}
                        {thumbnail && (
                            <div className="thumbnail-preview">
                                <img src={thumbnail} alt="썸네일 미리보기" className="thumbnail-image" />
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

                <div className="upload-progress-container">
                    <div className="upload-progress-track">
                        <div className="upload-progress-indicator" style={{ width: `${progress}%` }}></div>
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

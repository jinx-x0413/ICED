import React, { useState } from 'react';
import axios from 'axios';
import './InstallerDownload.css';

const baseURL = process.env.REACT_APP_BASE_URL;

const InstallerDownload = () => {
  const [downloading, setDownloading] = useState(false);
  const [progress, setProgress] = useState(0);
  const [error, setError] = useState(null);

  const handleDownload = async () => {
    setDownloading(true);
    setProgress(0);
    setError(null);

    try {
      const response = await axios({
        url: `${baseURL}/download/installer`,
        method: 'GET',
        responseType: 'blob',
        onDownloadProgress: (progressEvent) => {
          const percentCompleted = Math.round(
            (progressEvent.loaded * 100) / progressEvent.total
          );
          setProgress(percentCompleted);
        }
      });

      // 다운로드 링크 생성
      const url = window.URL.createObjectURL(new Blob([response.data]));
      const link = document.createElement('a');
      link.href = url;
      link.setAttribute('download', 'client-installer.exe');
      document.body.appendChild(link);
      link.click();
      link.remove();
      window.URL.revokeObjectURL(url);
      
      setDownloading(false);
    } catch (err) {
      console.error('다운로드 오류:', err);
      setError('다운로드 중 오류가 발생했습니다. 다시 시도해주세요.');
      setDownloading(false);
    }
  };

  return (
    <div className="installer-download">
      <h3>클라이언트 설치 프로그램</h3>
      <p>클라이언트 실행하기 위해 클라이언트 설치 프로그램을 다운로드하세요.</p>
      
      <button 
        className="download-button" 
        onClick={handleDownload}
        disabled={downloading}
      >
        {downloading ? '다운로드 중...' : '설치 프로그램 다운로드'}
      </button>
      
      {downloading && (
        <div className="progress-container">
          <div className="progress-bar">
            <div className="progress" style={{ width: `${progress}%` }}></div>
          </div>
          <span className="progress-text">{progress}%</span>
        </div>
      )}
      
      {error && (
        <div className="error-message">{error}</div>
      )}
      
      <div className="instructions">
        <h4>설치 안내</h4>
        <ol>
          <li>설치 프로그램을 다운로드합니다.</li>
          <li>다운로드한 프로그램을 실행합니다.</li>
          <li>설치 경로를 선택하고 설치를 진행합니다.</li>
          <li>설치가 완료되면 웹에서 '클라이언트 실행' 버튼을 클릭하여 클라언트를 실행할 수 있습니다.</li>
        </ol>
      </div>
    </div>
  );
};

export default InstallerDownload;
// components/Install.js
import React, { useState } from 'react';
import './css/install.css';

const Install = () => {
    const [downloading, setDownloading] = useState(false);
    const [downloadProgress, setDownloadProgress] = useState(0);

    const handleDownload = () => {
        setDownloading(true);
        // 다운로드 시작을 시뮬레이션하기 위한 코드
        setDownloadProgress(0);
        
        // 진행 상황을 시뮬레이션하기 위한 간단한 타이머
        const interval = setInterval(() => {
            setDownloadProgress(prev => {
                if (prev >= 100) {
                    clearInterval(interval);
                    setTimeout(() => {
                        setDownloading(false);
                        // 실제 다운로드 시작
                        window.location.href = 'http://localhost:8080/download/installer';
                    }, 500);
                    return 100;
                }
                return prev + 5;
            });
        }, 150);
    };

    return (
        <div className="install-container">
            <div className="install-header">
                <h2>ICED 클라이언트 설치</h2>
                <p className="subtitle">언리얼 엔진으로 제작된 가구 모델링을 위한 클라이언트입니다.</p>
            </div>

            <div className="install-content">
                <div className="install-section">
                    <h3>설치 안내</h3>
                    <div className="install-steps">
                        <div className="step">
                            <div className="step-number">1</div>
                            <div className="step-content">
                                <h4>다운로드</h4>
                                <p>아래 버튼을 클릭하여 설치 프로그램을 다운로드합니다.</p>
                            </div>
                        </div>
                        <div className="step">
                            <div className="step-number">2</div>
                            <div className="step-content">
                                <h4>설치 실행</h4>
                                <p>다운로드한 설치 파일을 실행하고 안내에 따라 설치를 진행합니다.</p>
                            </div>
                        </div>
                        <div className="step">
                            <div className="step-number">3</div>
                            <div className="step-content">
                                <h4>게임 실행</h4>
                                <p>설치가 완료되면 '클라이언트 실행' 버튼을 통해 클라이언트를 시작할 수 있습니다.</p>
                            </div>
                        </div>
                    </div>
                </div>

                <div className="install-section">
                    <h3>시스템 요구사항</h3>
                    <div className="requirements-container">
                        <div className="requirement">
                            <h4>최소 사양</h4>
                            <ul>
                                <li><strong>운영체제:</strong> Windows 10 64비트</li>
                                <li><strong>프로세서:</strong> Intel Core i5 또는 AMD Ryzen 5</li>
                                <li><strong>메모리:</strong> 8GB RAM</li>
                                <li><strong>그래픽:</strong> NVIDIA GTX 1050 또는 AMD Radeon RX 560</li>
                                <li><strong>저장공간:</strong> 10GB 이상의 여유 공간</li>
                            </ul>
                        </div>
                        <div className="requirement">
                            <h4>권장 사양</h4>
                            <ul>
                                <li><strong>운영체제:</strong> Windows 10/11 64비트</li>
                                <li><strong>프로세서:</strong> Intel Core i7 또는 AMD Ryzen 7</li>
                                <li><strong>메모리:</strong> 16GB RAM</li>
                                <li><strong>그래픽:</strong> NVIDIA GTX 1660 이상 또는 AMD Radeon RX 5600 XT 이상</li>
                                <li><strong>저장공간:</strong> SSD 20GB 이상의 여유 공간</li>
                            </ul>
                        </div>
                    </div>
                </div>

                <div className="download-section">
                    <button 
                        className={`download-button ${downloading ? 'downloading' : ''}`} 
                        onClick={handleDownload}
                        disabled={downloading}
                    >
                        {downloading ? '다운로드 중...' : '클라이언트 다운로드'}
                    </button>
                    {downloading && (
                        <div className="progress-container">
                            <div className="progress-bar">
                                <div className="progress" style={{ width: `${downloadProgress}%` }}></div>
                            </div>
                            <span className="progress-text">{downloadProgress}%</span>
                        </div>
                    )}
                    <p className="download-info">
                        파일 크기: 약 190MB | 버전: 1.0.0
                    </p>
                </div>

                <div className="install-section faq-section">
                    <h3>자주 묻는 질문</h3>
                    <div className="faq-item">
                        <h4>설치 중 오류가 발생하면 어떻게 해야 하나요?</h4>
                        <p>설치 중 오류가 발생할 경우, 관리자 권한으로 설치 프로그램을 실행해보세요.</p>
                    </div>
                    <div className="faq-item">
                        <h4>설치 완료 후 클라이언트가 실행되지 않아요.</h4>
                        <p>설치 위치를 확인 후 클라이언트 실행 파일이 차단되지 않았는지 확인하세요.</p>
                    </div>
                    <div className="faq-item">
                        <h4>기존 설치를 업데이트하려면 어떻게 해야 하나요?</h4>
                        <p>기존 설치본이 있는 경우, 동일한 설치 프로그램을 실행하면 자동으로 업데이트가 진행됩니다.</p>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default Install;

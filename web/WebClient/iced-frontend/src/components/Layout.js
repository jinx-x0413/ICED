// components/Layout.js
import React, { useState } from 'react';
import { useAuth } from '../AuthContext';
import { useNavigate, useLocation } from 'react-router-dom';
import './css/layout.css';

const Layout = ({ children }) => {
    const { state, dispatch } = useAuth();
    const user = state.user;
    const navigate = useNavigate();
    const location = useLocation();
    const [showInstallGuide, setShowInstallGuide] = useState(false);
    
    // 현재 경로에 따라 활성화된 메뉴 항목 결정
    const isActive = (path) => location.pathname === path;

    const logout = async () => {
        try {
            const res = await fetch("http://localhost:8080/logout", {
                method: "POST",
                credentials: "include"
            });

            if (res.ok) {
                localStorage.removeItem("accessToken");
                dispatch({ type: "LOGOUT" });
                alert("로그아웃 완료");
                navigate('/main');
            } else {
                throw new Error("로그아웃 실패");
            }
        } catch (error) {
            console.error(error);
            alert(error.message);
        }
    };

    const navigateTo = (path) => {
        navigate(path);
    };

    const launchClient = () => {
        try {
            // 프로토콜 호출
            window.location.href = 'iceduecli://start';
            
            // 오류 처리를 위한 타임아웃 설정
            setTimeout(() => {
                setShowInstallGuide(true);
            }, 1000);
        } catch (error) {
            console.error('프로토콜 호출 오류:', error);
            setShowInstallGuide(true);
        }
    };
    
    // 설치 프로그램 다운로드 버튼 클릭 시 install.js 페이지로 이동
    const downloadInstaller = () => {
        setShowInstallGuide(false); // 모달 닫기
        navigateTo('/install'); // install.js 페이지로 이동
    };

    return (
        <div className="layout-container">
            <nav className="navbar">
                <div className="menu">
                    <div className="logo" onClick={() => navigateTo('/main')}>
                        ICED
                    </div>
                    <ul className="nav-links">
                        <li className="nav-item">
                            <button 
                                className={`nav-button ${isActive('/install') ? 'active' : ''}`}
                                onClick={() => navigateTo('/install')}
                            >
                                클라이언트 설치
                            </button>
                        </li>
                        <li className="nav-item">
                            <button 
                                className={`nav-button ${isActive('/upload') ? 'active' : ''}`}
                                onClick={() => navigateTo('/upload')}
                            >
                                파일 업로드
                            </button>
                        </li>
                        <li className="nav-item">
                            <button 
                                className={`nav-button ${isActive('/mylist') ? 'active' : ''}`}
                                onClick={() => navigateTo('/mylist')}
                            >
                                업로드 목록 조회
                            </button>
                        </li>
                    </ul>
                </div>
                <div className="user-section">
                    <div className="action-buttons">
                        <button className="run-button" onClick={launchClient}>
                            클라이언트 실행
                        </button>
                        
                        {user ? (
                            <>
                                <div className="user-profile">
                                    <div className="user-avatar">
                                        
                                    </div>
                                    <span className="user-id">{user.userid}</span>
                                </div>
                                <button className="logout-button" onClick={logout}>
                                    로그아웃
                                </button>
                            </>
                        ) : (
                            <button className="login-button" onClick={() => navigateTo('/login')}>
                                로그인
                            </button>
                        )}
                    </div>
                </div>
            </nav>
            <main className="content">
                {children}
            </main>
            
            {showInstallGuide && (
                <div className="installation-guide">
                    <div className="guide-content">
                        <h3>클라이언트 실행이 안되나요?</h3>
                        <p>클라이언트 설치가 필요합니다.</p>
                        <button onClick={downloadInstaller} className="laydownload-button">설치 프로그램 다운로드</button>
                        <button onClick={() => setShowInstallGuide(false)} className="close-button">닫기</button>
                    </div>
                </div>
            )}
        </div>
    );
};

export default Layout;

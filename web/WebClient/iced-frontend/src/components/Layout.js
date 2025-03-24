// components/Layout.js
import React from 'react';
import { useAuth } from '../AuthContext';
import { useNavigate, useLocation } from 'react-router-dom';
import './css/layout.css';

const Layout = ({ children }) => {
    const { state, dispatch } = useAuth();
    const user = state.user;
    const navigate = useNavigate();
    const location = useLocation();
    
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

    const runClient = () => {
        window.electron.send('run-client');
        window.electron.receive('run-client-response', (message) => {
            alert(message);
        });
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
                        <button className="run-button" onClick={runClient}>
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
        </div>
    );
};

export default Layout;

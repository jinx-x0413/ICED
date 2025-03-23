// Main.js
import React from 'react';
import { useAuth } from '../AuthContext';
import { useNavigate } from 'react-router-dom'; // 추가: 페이지 이동을 위한 훅
import './css/main.css'

const Main = () => {
    const { state, dispatch } = useAuth();
    const user = state.user;
    const navigate = useNavigate(); // 추가: 페이지 이동을 위한 훅

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
            } else {
                throw new Error("로그아웃 실패");
            }
        } catch (error) {
            console.error(error);
            alert(error.message);
        }
    };

    // 로그인 페이지로 이동하는 함수
    const goToLogin = () => {
        navigate('/login');
    };

    return (
        <div style={styles.container}>
            <nav style={styles.navbar}>
                <div style={styles.menu}>
                    <button style={styles.menuItem}>클라이언트 설치</button>
                    <button style={styles.menuItem}>파일업로드</button>
                    <button style={styles.menuItem}>목록조회</button>
                </div>
                <div style={styles.userSection}>
                    <button style={styles.runButton}>클라이언트 실행</button>
                    {user ? (
                        <>
                            <span style={styles.userId}>{user.userid}님</span>
                            <button style={styles.logoutButton} onClick={logout}>로그아웃</button>
                        </>
                    ) : (
                        <button style={styles.loginButton} onClick={goToLogin}>로그인</button>
                    )}
                </div>
            </nav>
        </div>
    );
};

const styles = {
    container: {
        justifyContent: "left",
        fontFamily: 'Arial, sans-serif',
        width: '100%',
        margin: '0',
        padding: '0',
    },
    navbar: {
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'center',
        backgroundColor: '#f8f8f8',
        padding: '10px 20px',
        boxShadow: '0 2px 4px rgba(0, 0, 0, 0.1)',
        position: 'fixed',
        top: '0',
        width: '100%',
        zIndex: '1000',
    },
    menu: {
        display: 'flex',
    },
    menuItem: {
        marginRight: '10px',
        backgroundColor: '#4a6cf7',
        color: '#fff',
        border: 'none',
        padding: '10px 15px',
        borderRadius: '5px',
        cursor: 'pointer',
        fontSize: '14px', // 일관된 폰트 크기
    },
    userSection: {
        display: 'flex',
        alignItems: 'center',
    },
    userId: {
        marginRight: '10px',
        fontSize: '14px', // 일관된 폰트 크기
    },
    // 공통 버튼 스타일 (로그인/로그아웃)
    loginButton : {
        backgroundColor: '#4a6cf7', // 로그인 색상
        color: '#fff',
        border: 'none',
        padding: '10px 15px', // 다른 버튼과 동일한 패딩
        borderRadius: '5px',
        cursor: 'pointer',
        fontSize: '14px', // 일관된 폰트 크기
    },
    logoutButton: {
        backgroundColor: '#f44336', // 로그아웃 색상
        color: '#fff',
        border: 'none',
        padding: '10px 15px', // 다른 버튼과 동일한 패딩
        borderRadius: '5px',
        cursor: 'pointer',
        fontSize: '14px', // 일관된 폰트 크기
    },
    runButton: {
        backgroundColor: '#4CAF50',
        color: '#fff',
        border: 'none',
        padding: '10px 15px',
        borderRadius: '5px',
        cursor: 'pointer',
        marginRight: '10px',
        fontSize: '14px', // 일관된 폰트 크기
    }
};

export default Main;

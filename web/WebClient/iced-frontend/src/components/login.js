import React, { useState } from 'react';
import { useAuth } from '../AuthContext';
import './css/login.css';

const baseURL = process.env.REACT_APP_BASE_URL;
//const API_BASE = "http://localhost:8080";
const API_BASE = baseURL;

function Login() {
    const [isSignup, setIsSignup] = useState(false);
    const [form, setForm] = useState({
        email: "",
        userid: "",
        username: "",
        password: "",
        birthDate: "",
        school: "",
    });
    const [loginForm, setLoginForm] = useState({
        userid: "",
        password: "",
    });
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState("");

    const { dispatch } = useAuth();

    const toggleForm = () => {
        setIsSignup(!isSignup);
        setError("");
    };

    const handleInputChange = (e, isLogin = false) => {
        const { name, value } = e.target;
        isLogin
            ? setLoginForm((prev) => ({ ...prev, [name]: value }))
            : setForm((prev) => ({ ...prev, [name]: value }));
    };

    const join = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError("");
        
        try {
            const formData = new URLSearchParams(form);
            const res = await fetch(`${API_BASE}/join`, {
                method: "POST",
                body: formData,
            });
            
            const data = await res.json();
            
            if (!res.ok) {
                throw new Error(data.message || "회원가입 실패");
            }
            
            alert("회원가입 성공");
            setIsSignup(false);
        } catch (error) {
            console.error("회원가입 실패:", error);
            setError(error.message || "회원가입 중 오류가 발생했습니다.");
        } finally {
            setLoading(false);
        }
    };

    const login = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError("");
        
        try {
            const formData = new URLSearchParams(loginForm);
            const res = await fetch(`${API_BASE}/login`, {
                method: "POST",
                body: formData,
                credentials: "include",
            });

            if (!res.ok) {
                const data = await res.json();
                throw new Error(data.message || "로그인 실패");
            }

            const accessToken = res.headers.get("Authorization");
            if (accessToken) {
                const token = accessToken.startsWith("Bearer ") ? accessToken : `Bearer ${accessToken}`;
                localStorage.setItem("accessToken", token);
                dispatch({ type: "LOGIN", payload: { userid: loginForm.userid } });
            }
        } catch (error) {
            console.error(error);
            setError(error.message || "로그인 실패");
        } finally {
            setLoading(false);
        }
    };

    return (
        <div className="auth-container">
            <div className="auth-card">
                <div className="auth-header">
                    <h2>{isSignup ? "회원가입" : "로그인"}</h2>
                    {error && <p className="error-message">{error}</p>}
                </div>
                
                {isSignup ? (
                    <form className="auth-form" onSubmit={join}>
                        <div className="form-group">
                            <input 
                                type="text" 
                                name="username" 
                                placeholder="이름" 
                                onChange={handleInputChange} 
                                required 
                            />
                        </div>
                        <div className="form-group">
                            <input 
                                type="date" 
                                name="birthDate" 
                                placeholder="생년 월일" 
                                onChange={handleInputChange} 
                                required 
                            />
                        </div>
                        <div className="form-group">
                            <input 
                                type="text" 
                                name="school" 
                                placeholder="학교" 
                                onChange={handleInputChange} 
                            />
                        </div>
                        <div className="form-group">
                            <input 
                                type="email" 
                                name="email" 
                                placeholder="ID(이메일)" 
                                onChange={handleInputChange} 
                                required 
                            />
                        </div>
                        <div className="form-group">
                            <input 
                                type="password" 
                                name="password" 
                                placeholder="비밀번호" 
                                onChange={handleInputChange} 
                                required 
                                minLength="6"
                            />
                        </div>
                        <button type="submit" className="auth-button" disabled={loading}>
                            {loading ? "처리 중..." : "회원가입"}
                        </button>
                    </form>
                ) : (
                    <form className="auth-form" onSubmit={login}>
                        <div className="form-group">
                            <input 
                                type="text" 
                                name="userid" 
                                placeholder="아이디" 
                                onChange={(e) => handleInputChange(e, true)} 
                                required 
                            />
                        </div>
                        <div className="form-group">
                            <input 
                                type="password" 
                                name="password" 
                                placeholder="비밀번호" 
                                onChange={(e) => handleInputChange(e, true)} 
                                required 
                            />
                        </div>
                        <button type="submit" className="auth-button" disabled={loading}>
                            {loading ? "로그인 중..." : "로그인"}
                        </button>
                    </form>
                )}
                
                <div className="auth-footer">
                    <span className="toggle-form-btn" onClick={toggleForm}>
                        {isSignup ? "계정이 있으신가요? 로그인" : "계정이 없으신가요? 회원가입"}
                    </span>
                </div>
            </div>
        </div>
    );
}

export default Login;

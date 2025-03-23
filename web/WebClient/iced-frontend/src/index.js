import React from 'react';
import ReactDOM from 'react-dom/client';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';
import './index.css';
import App from './App';
import { AuthProvider } from './AuthContext';
import ProtectedRoute from './components/ProtectedRoute';
import PublicRoute from './components/PublicRoute';
import LoginPage from './components/login'; // 로그인 페이지 컴포넌트
import Main from './components/main'; // 메인 페이지 컴포넌트
import Upload from './components/upload'; // 업로드 페이지 컴포넌트

const root = ReactDOM.createRoot(document.getElementById('root'));

root.render(
    <React.StrictMode>
        <AuthProvider>
            <Router>
                <Routes>
                    <Route path="/" element={<App />} />
                    <Route path="/login" element={<PublicRoute><LoginPage /></PublicRoute>} /> {/* 로그인 하지 않은경우만 이동가능 */}
                    <Route path="/main" element={<Main />} /> 
                    <Route path="/upload" element={<ProtectedRoute><Upload /></ProtectedRoute>} /> {/* 로그인 한경우에만 이동가능 */}
                    <Route path="*" element={<Navigate to="/" replace />} />
                </Routes>
            </Router>
        </AuthProvider>
    </React.StrictMode>
);

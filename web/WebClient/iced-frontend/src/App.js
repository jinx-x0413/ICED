// App.js
import React from 'react';
import { Navigate } from 'react-router-dom';
import { useAuth } from './AuthContext';

const App = () => {
    const { state } = useAuth();
    const user = state.user;

    if (user) {
        // 사용자가 로그인되어 있으면, 메인 페이지로 리다이렉트
        return <Navigate to="/main" replace />;
    }

    return (
        <div>
            {/* 로그인되지 않은 사용자에게 보여줄 기본 콘텐츠 */}
            <h1>Welcome to Our Application</h1>
            <p>Please log in to access more features.</p>
        </div>
    );
};

export default App;

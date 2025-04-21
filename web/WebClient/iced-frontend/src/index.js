import React from 'react';
import ReactDOM from 'react-dom/client';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';
import './index.css';
import App from './App';
import { AuthProvider } from './AuthContext';
import ProtectedRoute from './components/ProtectedRoute';
import PublicRoute from './components/PublicRoute';
import Layout from './components/Layout';
import LoginPage from './components/login';
import Main from './components/main';
import Upload from './components/upload';
import Install from './components/install';
import MyList from './components/mylist';
import Cart from './components/cart';


const root = ReactDOM.createRoot(document.getElementById('root'));

root.render(
    <React.StrictMode>
        <AuthProvider>
            <Router>
                <Routes>
                    <Route path="/login" element={<PublicRoute><LoginPage /></PublicRoute>} />
                    <Route path="/" element={<Layout><App /></Layout>} />
                    <Route path="/main" element={<Layout><Main /></Layout>} />
                    <Route path="/upload" element={<Layout><ProtectedRoute><Upload /></ProtectedRoute></Layout>} />
                    <Route path="/install" element={<Layout><Install /></Layout>} />
                    <Route path="/mylist" element={<Layout><ProtectedRoute><MyList /></ProtectedRoute></Layout>} />
                    <Route path="/cart" element={<Layout><ProtectedRoute><Cart /></ProtectedRoute></Layout>} />

                    <Route path="*" element={<Navigate to="/" replace />} />
                </Routes>
            </Router>
        </AuthProvider>
    </React.StrictMode>
);

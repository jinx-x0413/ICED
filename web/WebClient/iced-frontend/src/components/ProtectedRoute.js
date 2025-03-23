// ProtectedRoute.js
import React from 'react';
import { Navigate } from 'react-router-dom';
import { useAuth } from '../AuthContext';

const ProtectedRoute = ({ children }) => {
    const { state } = useAuth();
    return state.user ? children : <Navigate to="/login" replace />;
};

export default ProtectedRoute;

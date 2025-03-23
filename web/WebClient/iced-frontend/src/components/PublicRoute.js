// PublicRoute.js
import React from 'react';
import { Navigate } from 'react-router-dom';
import { useAuth } from '../AuthContext';

const PublicRoute = ({ children }) => {
    const { state } = useAuth();
    return state.user ? <Navigate to="/main" replace /> : children;
};

export default PublicRoute;

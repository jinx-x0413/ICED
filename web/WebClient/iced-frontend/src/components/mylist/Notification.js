// components/mylist/Notification.js
import React from 'react';

const Notification = ({ message, type, visible, position = 'top-right' }) => {
    if (!visible) return null;
    
    return (
        <div className={`mylist-notification ${type} ${position}`}>
            {type === 'success' && <span className="mylist-icon">✅</span>}
            {type === 'error' && <span className="mylist-icon">❌</span>}
            {type === 'warning' && <span className="mylist-icon">⚠️</span>}
            <span className="mylist-message">{message}</span>
        </div>
    );
};

export default Notification;

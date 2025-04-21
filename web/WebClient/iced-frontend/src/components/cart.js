import React, { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import './css/cart.css';

// 토스트 알림 컴포넌트
const Toast = ({ message, isVisible, type }) => {
    if (!isVisible) return null;
    
    return (
        <div className={`toast-notification ${type}`}>
            {message}
        </div>
    );
};

// 확인 모달 컴포넌트
const ConfirmModal = ({ isOpen, onClose, onConfirm, message, title }) => {
    if (!isOpen) return null;
    
    return (
        <div className="modal-overlay">
            <div className="modal-content confirm-modal">
                <div className="modal-header">
                    <h2>{title || '확인'}</h2>
                    <button className="close-button" onClick={onClose}>&times;</button>
                </div>
                <div className="modal-body">
                    <p>{message}</p>
                </div>
                <div className="modal-actions">
                    <button className="btn-cancel" onClick={onClose}>취소</button>
                    <button className="btn-confirm" onClick={onConfirm}>확인</button>
                </div>
            </div>
        </div>
    );
};

const Cart = () => {
    const navigate = useNavigate();
    
    // 상태
    const [cartItems, setCartItems] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);
    const [toast, setToast] = useState({ visible: false, message: '', type: 'success' });
    const [confirmModal, setConfirmModal] = useState({
        isOpen: false,
        message: '',
        title: '',
        onConfirm: () => {}
    });
    
    // 토스트 알림 표시 함수
    const showToast = (message, type = 'success') => {
        setToast({ visible: true, message, type });
        setTimeout(() => {
            setToast(prev => ({ ...prev, visible: false }));
        }, 3000);
    };
    
    // 장바구니 항목 로드 함수
    const loadCartItems = async () => {
        try {
            setLoading(true);
            
            // JWT 토큰 가져오기 (인증 관련 처리는 외부에서 이루어진다고 가정)
            const token = localStorage.getItem('accessToken');
            
            // API 호출
            const response = await fetch('http://localhost:8080/api/cart', {
                headers: {
                    'Authorization': token
                }
            });
            
            if (!response.ok) {
                throw new Error('장바구니 조회 실패');
            }
            
            const data = await response.json();
            console.log('장바구니 응답 데이터:', data); // 디버깅용 로그
            
            // 응답 구조 검사 및 처리
            if (data && data.items && Array.isArray(data.items)) {
                setCartItems(data.items);
            } else if (Array.isArray(data)) {
                // 배열 형태로 직접 반환된 경우
                setCartItems(data);
            } else {
                console.error('예상치 못한 응답 형식:', data);
                setCartItems([]);
            }
            
        } catch (error) {
            console.error('장바구니 로드 오류:', error);
            setError(error.message || '장바구니를 불러오는 중 오류가 발생했습니다');
        } finally {
            setLoading(false);
        }
    };
    
    // 개별 항목 삭제 함수
    const removeCartItem = async (fileId) => {
        try {
            const token = localStorage.getItem('accessToken');
            
            const response = await fetch(`http://localhost:8080/api/cart/${fileId}`, {
                method: 'DELETE',
                headers: {
                    'Authorization': token
                }
            });
            
            if (!response.ok) {
                throw new Error('항목 삭제 실패');
            }
            
            // 성공 시 상태 업데이트
            setCartItems(prev => prev.filter(item => item.fileId !== fileId));
            showToast('장바구니에서 삭제되었습니다');
            
        } catch (error) {
            console.error('항목 삭제 오류:', error);
            showToast(error.message || '항목 삭제 중 오류가 발생했습니다', 'error');
        }
    };
    
    // 장바구니 비우기 함수
    const clearCart = async () => {
        try {
            const token = localStorage.getItem('accessToken');
            
            const response = await fetch('http://localhost:8080/api/cart', {
                method: 'DELETE',
                headers: {
                    'Authorization': token
                }
            });
            
            if (!response.ok) {
                throw new Error('장바구니 비우기 실패');
            }
            
            // 성공 시 상태 업데이트
            setCartItems([]);
            showToast('장바구니가 비워졌습니다');
            
        } catch (error) {
            console.error('장바구니 비우기 오류:', error);
            showToast(error.message || '장바구니 비우기 중 오류가 발생했습니다', 'error');
        }
    };
    
    // 항목 삭제 확인 모달 표시
    const confirmRemoveItem = (fileId, fileName) => {
        setConfirmModal({
            isOpen: true,
            title: '항목 삭제',
            message: `"${fileName || '선택한 항목'}"을(를) 장바구니에서 삭제하시겠습니까?`,
            onConfirm: () => {
                removeCartItem(fileId);
                setConfirmModal(prev => ({ ...prev, isOpen: false }));
            }
        });
    };
    
    // 장바구니 비우기 확인 모달 표시
    const confirmClearCart = () => {
        setConfirmModal({
            isOpen: true,
            title: '장바구니 비우기',
            message: '장바구니의 모든 항목을 삭제하시겠습니까?',
            onConfirm: () => {
                clearCart();
                setConfirmModal(prev => ({ ...prev, isOpen: false }));
            }
        });
    };
    
    // 컴포넌트 마운트 시 장바구니 항목 로드
    useEffect(() => {
        loadCartItems();
    }, []);
    
    // 날짜 포맷 함수
    const formatDate = (dateString) => {
        if (!dateString) return '날짜 정보 없음';
        
        const date = new Date(dateString);
        return date.toLocaleDateString('ko-KR', {
            year: 'numeric',
            month: 'long',
            day: 'numeric'
        });
    };
    
    // 파일 크기 포맷 함수
    const formatFileSize = (size) => {
        if (size === null || size === undefined) return '크기 정보 없음';
        return `${size.toFixed(2)} MB`;
    };

    return (
        <div className="cart-container">
            {/* 토스트 알림 */}
            <Toast 
                message={toast.message} 
                isVisible={toast.visible} 
                type={toast.type}
            />
            
            {/* 확인 모달 */}
            <ConfirmModal
                isOpen={confirmModal.isOpen}
                onClose={() => setConfirmModal(prev => ({ ...prev, isOpen: false }))}
                onConfirm={confirmModal.onConfirm}
                title={confirmModal.title}
                message={confirmModal.message}
            />
            
            <div className="cart-header">
                <h1>내 장바구니</h1>
                {cartItems.length > 0 && (
                    <button 
                        className="clear-cart-btn"
                        onClick={confirmClearCart}
                    >
                        장바구니 비우기
                    </button>
                )}
            </div>
            
            {loading ? (
                <div className="loading-container">
                    <div className="loading-spinner"></div>
                    <p>장바구니 불러오는 중...</p>
                </div>
            ) : error ? (
                <div className="error-message">
                    <i className="error-icon">❌</i>
                    <p>{error}</p>
                </div>
            ) : cartItems.length === 0 ? (
                <div className="empty-cart">
                    <div className="empty-cart-icon">🛒</div>
                    <h2>장바구니가 비어있습니다</h2>
                    <p>파일 목록에서 파일을 장바구니에 추가해보세요.</p>
                    <button 
                        className="go-to-files-btn"
                        onClick={() => navigate('/main')}
                    >
                        파일 목록으로 이동
                    </button>
                </div>
            ) : (
                <div className="cart-items-container">
                    <div className="cart-summary">
                        <p>총 <span className="item-count">{cartItems.length}</span>개의 항목</p>
                    </div>
                    
                    <div className="cart-items-list">
                        {cartItems.map((item) => (
                            <div className="cart-item" key={item.id || item.fileId}>
                                <div className="item-thumbnail">
                                    {item.thumbnailUri ? (
                                        <img 
                                            src={item.thumbnailUri.startsWith('http') 
                                                ? item.thumbnailUri 
                                                : `http://localhost:8080${item.thumbnailUri}`}
                                            alt={item.fileName || 'File thumbnail'} 
                                        />
                                    ) : (
                                        <div className="no-thumbnail">
                                            <span>No Image</span>
                                        </div>
                                    )}
                                </div>
                                
                                <div className="item-details">
                                    <h3 className="item-name">
                                        {item.description || '파일명 없음'}
                                    </h3>
                                    <div className="item-meta">
                                        <span className="item-size">{formatFileSize(item.size)}</span>
                                        <span className="item-date">추가일: {formatDate(item.addedAt)}</span>
                                    </div>
                                </div>
                                
                                <div className="item-actions">
                                    <button 
                                        className="remove-item-btn"
                                        onClick={() => confirmRemoveItem(item.fileId || item.id, item.fileName)}
                                    >
                                        제거
                                    </button>
                                </div>
                            </div>
                        ))}
                    </div>
                </div>
            )}
        </div>
    );
};

export default Cart;

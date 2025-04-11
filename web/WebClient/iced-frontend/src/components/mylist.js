// components/Mylist.js
import React, { useState, useEffect, useCallback, useMemo, useRef } from 'react';
import Notification from './mylist/Notification';
import DetailModal from './mylist/DetailModal';
import EditModal from './mylist/EditModal';
import './css/mylist.css';

const Mylist = () => {
    const [files, setFiles] = useState([]);
    const [loading, setLoading] = useState(true);
    const [sortOrder, setSortOrder] = useState('newest'); // 정렬 기준 (newest, oldest, name)
    const [error, setError] = useState(null);
    const [userId, setUserId] = useState('');
    const [notification, setNotification] = useState({ message: '', type: '', visible: false });
    const [editModalOpen, setEditModalOpen] = useState(false);
    const [editingFile, setEditingFile] = useState(null);
    const [detailModalOpen, setDetailModalOpen] = useState(false);
    const [viewingFile, setViewingFile] = useState(null);
    const [searchTerm, setSearchTerm] = useState('');
    const [pageLoading, setPageLoading] = useState(true);

    // 페이지네이션 상태
    const [currentPage, setCurrentPage] = useState(1);
    const [itemsPerPage, setItemsPerPage] = useState(10); // 기본값 설정 (나중에 계산됨)
    const [pageData, setPageData] = useState([]); // 현재 페이지 데이터

    // 파일 리스트 컨테이너에 대한 ref
    const fileListRef = useRef(null);
    const containerRef = useRef(null);
    const initialized = useRef(false);
    const resizeTimeoutRef = useRef(null);

    // 디바운스 함수 - 리사이즈나 검색 시 연속 호출 방지
    const debounce = (func, delay) => {
        return (...args) => {
            if (resizeTimeoutRef.current) {
                clearTimeout(resizeTimeoutRef.current);
            }
            resizeTimeoutRef.current = setTimeout(() => {
                func(...args);
            }, delay);
        };
    };

    // JWT 토큰에서 사용자 ID 추출
    const extractUserIdFromToken = useCallback((token) => {
        try {
            const parts = token.split('.');
            if (parts.length !== 3) {
                console.error('Invalid token format');
                return null;
            }
            
            const payload = JSON.parse(atob(parts[1].replace(/-/g, '+').replace(/_/g, '/')));
            return payload.userid || payload.sub || payload.user_id || payload.id;
        } catch (error) {
            console.error('JWT 디코딩 오류:', error);
            return null;
        }
    }, []);

    // 사용자 정보 가져오기
    useEffect(() => {
        const token = localStorage.getItem('accessToken') || 
            "Bearer eyJhbGciOiJIUzI1NiJ9.eyJjYXRlZ29yeSI6IkF1dGhvcml6YXRpb24iLCJ1c2VyaWQiOiJhZG1pbiIsInJvbGUiOiJST0xFX0FETUlOIiwiaWF0IjoxNzQ0MzgyNjI0LCJleHAiOjE3NDQzOTM0MjR9.0UCgyoGIQ_mQFqG27z57J-7Rvzc6iK7gm3vR3PDIkqYdlrj";
        
        localStorage.setItem('accessToken', token);
        
        const jwtToken = token.startsWith('Bearer ') ? token.substring(7) : token;
        
        const id = extractUserIdFromToken(jwtToken);
        if (id) {
            setUserId(id);
        } else {
            setError('토큰에서 사용자 ID를 추출할 수 없습니다.');
            setLoading(false);
            setPageLoading(false);
        }
    }, [extractUserIdFromToken]);

    // 사용자의 파일 목록 가져오기
    useEffect(() => {
        if (!userId) return;

        const fetchUserFiles = async () => {
            try {
                const token = localStorage.getItem("accessToken");
                setLoading(true);
                setPageLoading(true);

                const response = await fetch(`http://localhost:8080/api/files/user/${userId}`, {
                    headers: {
                        'Authorization': token && (token.startsWith('Bearer ') ? token : `Bearer ${token}`)
                    },
                    credentials: 'include' // 쿠키 포함
                });
                
                if (!response.ok) {
                    throw new Error('서버 응답 오류: ' + response.status);
                }
                const data = await response.json();
                setFiles(data || []);
                setLoading(false);
            } catch (error) {
                console.error('Error fetching files:', error);
                setError(error.message);
                setLoading(false);
                setPageLoading(false);
            }
        };

        fetchUserFiles();
    }, [userId]);

    // 그리드 아이템 계산 - DOM이 로드된 후 실행
    useEffect(() => {
        // DOM이 완전히 로드된 후 계산을 위해 setTimeout 사용
        const calculateGridItems = () => {
            if (!fileListRef.current || !containerRef.current) return;
            
            // 컨테이너 크기 계산
            const containerWidth = fileListRef.current.clientWidth;
            
            // 사용 가능한 전체 높이 계산
            const containerRect = containerRef.current.getBoundingClientRect();
            const containerTop = containerRect.top;
            const windowHeight = window.innerHeight;
            const availableHeight = windowHeight - containerTop - 100; // 하단 여백 100px
            
            // 아이템 크기 정의
            const itemWidth = 300; // 아이템 기본 너비
            const itemHeight = 350; // 아이템 기본 높이 (썸네일 + 정보)
            
            // 행과 열 계산
            const itemsPerRow = Math.max(1, Math.floor(containerWidth / itemWidth))-1;
            const rowsPerPage = Math.max(1, Math.floor(availableHeight / itemHeight));
            
            // 최종 표시할 아이템 수 계산
            const totalItems = itemsPerRow * rowsPerPage;
            
            console.log('계산된 그리드 아이템:', {
                containerWidth,
                availableHeight,
                itemWidth,
                itemHeight,
                itemsPerRow,
                rowsPerPage,
                totalItems
            });
            
            // 이전과 다를 때만 업데이트
            if (itemsPerPage !== totalItems) {
                setItemsPerPage(totalItems);
            }
            
            initialized.current = true;
            
            // 스켈레톤 UI가 보일 시간을 주기 위해 지연 후 로딩 상태 해제
            setTimeout(() => {
                setPageLoading(false);
            }, 500);
        };
        
        // 첫 렌더링 후 약간의 지연을 두고 계산
        const timeoutId = setTimeout(calculateGridItems, 100);
        
        // 윈도우 크기 변경 시 재계산 - 디바운스 적용
        const handleResize = debounce(() => {
            // 리사이즈 시작 시 로딩 상태 활성화
            setPageLoading(true);
            calculateGridItems();
        }, 300);
        
        window.addEventListener('resize', handleResize);
        
        return () => {
            window.removeEventListener('resize', handleResize);
            clearTimeout(timeoutId);
            if (resizeTimeoutRef.current) {
                clearTimeout(resizeTimeoutRef.current);
            }
        };
    }, [itemsPerPage]);

    // 검색 필터링 및 정렬
    const filteredFiles = useMemo(() => {
        // 먼저 검색어로 필터링
        const filtered = files.filter(file => {
            return file.description && 
                file.description.toLowerCase().includes(searchTerm.toLowerCase());
        });
        
        // 정렬 기준에 따라 정렬
        return [...filtered].sort((a, b) => {
            switch (sortOrder) {
                case 'newest':
                    return new Date(b.createdAt || 0) - new Date(a.createdAt || 0);
                case 'oldest':
                    return new Date(a.createdAt || 0) - new Date(b.createdAt || 0);
                case 'name':
                    return (a.description || '').localeCompare(b.description || '');
                default:
                    return 0;
            }
        });
    }, [files, searchTerm, sortOrder]); // sortOrder를 의존성 배열에 추가

    // 정렬 변경 핸들러 추가
    const handleSortChange = (e) => {
        // 정렬 변경 시 로딩 상태 활성화
        setPageLoading(true);
        // 정렬 방식 변경
        setSortOrder(e.target.value);
        // 첫 페이지로 리셋
        setCurrentPage(1);
    };
    // 정렬 기준 변경 시 페이지 리셋 및 로딩 처리
    useEffect(() => {
        // 검색 후 데이터 로딩이 완료되면 로딩 상태 해제
        const timer = setTimeout(() => {
            setPageLoading(false);
        }, 500);
        
        return () => clearTimeout(timer);
    }, [sortOrder]); // sortOrder 의존성 추가
    // 검색 핸들러 - 로딩 상태 추가
    const handleSearch = (e) => {
        // 검색어 변경 시 로딩 상태 시작
        setPageLoading(true);
        setSearchTerm(e.target.value);
    };
    
    // 검색어 변경 시 페이지 리셋 및 로딩 처리
    useEffect(() => {
        setCurrentPage(1);
        
        // 검색 후 데이터 로딩이 완료되면 로딩 상태 해제
        const timer = setTimeout(() => {
            setPageLoading(false);
        }, 500);
        
        return () => clearTimeout(timer);
    }, [searchTerm, itemsPerPage]);
    
    // 그리드 크기 변경 시 페이지 리셋
    useEffect(() => {
        if (initialized.current) {
            setCurrentPage(1);
        }
    }, [itemsPerPage]);
    
    // 페이지네이션 계산 - 0으로 나누기 방지
    const totalPages = itemsPerPage > 0 ? Math.ceil(filteredFiles.length / itemsPerPage) : 0;

    // 페이지 데이터 계산
    useEffect(() => {
        // 로딩 중이거나 아이템 수가 계산되지 않았거나 필터링된 파일이 없으면 데이터 설정 안 함
        if (!itemsPerPage || filteredFiles.length === 0) {
            setPageLoading(false);
            return;
        }
        
        const loadPageData = async () => {
            try {
                // 페이지 데이터 계산
                const indexOfLastItem = currentPage * itemsPerPage;
                const indexOfFirstItem = indexOfLastItem - itemsPerPage;
                const currentFilesData = filteredFiles.slice(indexOfFirstItem, indexOfFirstItem + itemsPerPage);
                
                // 썸네일 URL 추가
                const filesWithThumbnails = currentFilesData.map(file => {
                    const thumbnailUrl = file.thumbnailUri 
                        ? `http://localhost:8080${file.thumbnailUri}` 
                        : `https://via.placeholder.com/300x200?text=${encodeURIComponent(file.description || 'No Image')}`;
                    
                    return {
                        ...file,
                        thumbnailUri: thumbnailUrl
                    };
                });
                
                // 약간의 지연을 주어 로딩 상태가 보이도록 함
                setTimeout(() => {
                    setPageData(filesWithThumbnails);
                    setPageLoading(false);
                }, 500);
                
            } catch (error) {
                console.error('페이지 데이터 로드 실패:', error);
                setPageLoading(false);
            }
        };
        
        // 페이지 데이터 로드
        loadPageData();
        
    }, [currentPage, itemsPerPage, filteredFiles]);

    // 파일 삭제 함수 - 인증 헤더 추가
    const handleDeleteFile = async (fileId) => {
        if (!window.confirm('정말로 이 파일을 삭제하시겠습니까? 이 작업은 되돌릴 수 없습니다.')) {
            return;
        }

        try {
            // LocalStorage에서 토큰 가져오기
            const token = localStorage.getItem("accessToken");
            
            const response = await fetch(`http://localhost:8080/api/files/${fileId}`, {
                method: 'DELETE',
                headers: {
                    'Authorization': token && (token.startsWith('Bearer ') ? token : `Bearer ${token}`)
                },
                credentials: 'include' // 쿠키 포함
            });

            if (!response.ok) {
                throw new Error('서버 응답 오류: ' + response.status);
            }

            // 삭제 성공 시 목록에서 해당 파일 제거
            setFiles(files.filter(file => file.id !== fileId));
            showNotification('파일이 성공적으로 삭제되었습니다.', 'success');
        } catch (error) {
            console.error('파일 삭제 실패:', error);
            showNotification('파일 삭제에 실패했습니다: ' + error.message, 'error');
        }
    };

    // 파일 수정 모달 열기
    const handleEditFile = (file) => {
        setEditingFile(file);
        setEditModalOpen(true);
    };

    // 파일 상세보기 모달 열기
    const handleViewDetails = (file) => {
        setViewingFile(file);
        setDetailModalOpen(true);
    };

    // 파일 수정 처리
    const handleUpdateFile = async (fileId, formData) => {
        try {
            // LocalStorage에서 토큰 가져오기
            const token = localStorage.getItem("accessToken");
            
            const response = await fetch(`http://localhost:8080/api/files/${fileId}/update`, {
                method: 'POST',
                headers: {
                    'Authorization': token && (token.startsWith('Bearer ') ? token : `Bearer ${token}`)
                },
                body: formData,
                credentials: 'include' // 쿠키 포함
            });

            if (!response.ok) {
                throw new Error('서버 응답 오류: ' + response.status);
            }

            // 모달 닫기
            setEditModalOpen(false);
            
            // 로딩 상태 활성화
            setPageLoading(true);
            
            // 서버에서 최신 데이터 다시 가져오기 - 역시 인증 헤더 추가
            const userFilesResponse = await fetch(`http://localhost:8080/api/files/user/${userId}`, {
                headers: {
                    'Authorization': token && (token.startsWith('Bearer ') ? token : `Bearer ${token}`)
                },
                credentials: 'include' // 쿠키 포함
            });
            
            if (!userFilesResponse.ok) {
                throw new Error('서버 응답 오류: ' + userFilesResponse.status);
            }
            
            const updatedFiles = await userFilesResponse.json();
            setFiles(updatedFiles || []);
            
            // 성공 메시지 (중앙에 표시)
            showNotification('파일 수정이 완료되었습니다.', 'success', 'center');
        } catch (error) {
            console.error('파일 업데이트 실패:', error);
            showNotification('파일 업데이트에 실패했습니다: ' + error.message, 'error');
        } finally {
            // 페이지 로딩 상태 해제 (데이터 다시 불러온 후)
            setTimeout(() => {
                setPageLoading(false);
            }, 500);
        }
    };


    // 알림 메시지 표시 함수 수정 (position 매개변수 추가)
    const showNotification = (message, type = 'success', position = 'top-right') => {
        setNotification({ message, type, visible: true, position });
        
        // 3초 후 자동으로 알림 닫기
        setTimeout(() => {
            setNotification(prev => ({ ...prev, visible: false }));
        }, 3000);
    };


    // 페이지 변경 핸들러 - 로딩 상태 추가
    const paginate = (pageNumber) => {
        if (pageNumber === currentPage) return;
        
        // 페이지 변경 시 즉시 이전 데이터 지우고 로딩 상태 표시
        setPageLoading(true);
        setCurrentPage(pageNumber);
    };
    
    // 이전 페이지, 다음 페이지 핸들러
    const goToPreviousPage = () => {
        if (currentPage > 1) {
            paginate(currentPage - 1);
        }
    };
    
    const goToNextPage = () => {
        if (currentPage < totalPages) {
            paginate(currentPage + 1);
        }
    };
    
    // 날짜 포맷 함수 (YYYY-MM-DD 부분만 추출)
    const formatDate = (dateString) => {
        if (!dateString) return 'Unknown date';
        
        // 정규식을 사용하여 YYYY-MM-DD 부분만 추출
        const match = dateString.match(/^(\d{4}-\d{2}-\d{2})/);
        return match ? match[1] : dateString;
    };

    return (
        <div className="mylist-main-container" ref={containerRef}>
            <Notification 
                message={notification.message}
                type={notification.type}
                visible={notification.visible}
                position={notification.position}
            />
                    
        {/* 검색 및 정렬 영역 */}
        <div className="mylist-controls-container">
            <div className="mylist-search-box">
                <input
                    type="text"
                    placeholder="파일 검색..."
                    value={searchTerm}
                    onChange={handleSearch}
                />
                <span className="mylist-search-icon">🔍</span>
            </div>
            
            <div className="mylist-sort-control">
                <label htmlFor="sort-order">정렬: </label>
                <select 
                    id="sort-order" 
                    value={sortOrder}
                    onChange={handleSortChange} 
                >
                    <option value="newest">최신순</option>
                    <option value="oldest">오래된순</option>
                    <option value="name">이름순</option>
                </select>
            </div>
        </div>

            {/* 상태 표시 */}
            <div className="mylist-status-bar">
                <span>총 {filteredFiles.length}개의 파일</span>
            </div>

            {error ? (
                <div className="mylist-error-message">
                    <i className="mylist-error-icon">❌</i>
                    <p>파일 목록을 불러오는데 실패했습니다: {error}</p>
                </div>
            ) : filteredFiles.length === 0 && !loading && !pageLoading ? (
                <div className="mylist-empty-state">
                    <div className="mylist-empty-icon">📂</div>
                    <h3>{searchTerm ? '검색 결과가 없습니다' : '업로드한 파일이 없습니다'}</h3>
                    <p>{searchTerm ? '다른 검색어로 시도해보세요.' : '새 파일을 업로드하여 목록을 채워보세요.'}</p>
                </div>
            ) : (
                <>
                    <div ref={fileListRef} className="mylist-file-grid">
                        {pageLoading || loading ? (
                            // 페이지 로딩 중일 때 스켈레톤 UI 표시
                            Array.from({ length: itemsPerPage || 6 }).map((_, index) => (
                                <div className="mylist-file-item mylist-skeleton" key={`skeleton-${index}`}>
                                    <div className="mylist-file-thumbnail mylist-skeleton-image"></div>
                                    <div className="mylist-file-info">
                                        <div className="mylist-skeleton-text"></div>
                                        <div className="mylist-skeleton-text mylist-skeleton-text-short"></div>
                                        <div className="mylist-file-actions">
                                            <div className="mylist-skeleton-button"></div>
                                            <div className="mylist-skeleton-button"></div>
                                        </div>
                                    </div>
                                </div>
                            ))
                        ) : (
                            pageData.map((file, index) => (
                                <div className="mylist-file-item" key={index}>
                                    <div className="mylist-file-thumbnail">
                                        <img 
                                            src={file.thumbnailUri} 
                                            alt={file.description || 'Unknown file'} 
                                        />
                                    </div>
                                    <div className="mylist-file-info">
                                        <p className="mylist-file-description">{file.description || 'No description'}</p>
                                        <div className="mylist-file-details">
                                            <span className="mylist-file-size">{file.size ? `${file.size.toFixed(2)} MB` : 'Unknown size'}</span>
                                            <span className="mylist-file-date">{formatDate(file.createdAt)}</span>
                                        </div>
                                        <div className="mylist-file-actions">
                                            <button className="mylist-action-button mylist-preview" onClick={() => handleViewDetails(file)}>상세보기</button>
                                            <button className="mylist-action-button mylist-edit" onClick={() => handleEditFile(file)}>수정</button>
                                            <button className="mylist-action-button mylist-delete" onClick={() => handleDeleteFile(file.id)}>삭제</button>
                                        </div>
                                    </div>
                                </div>
                            ))
                        )}
                    </div>
                    
                    {/* 페이지네이션 UI */}
                    {totalPages > 1 && (
                        <div className="mylist-pagination">
                            <button 
                                className="mylist-pagination-button mylist-prev"
                                onClick={goToPreviousPage}
                                disabled={currentPage === 1 || pageLoading}
                            >
                                &laquo;
                            </button>
                            
                            {[...Array(totalPages)].map((_, index) => (
                                <button
                                    key={index}
                                    onClick={() => paginate(index + 1)}
                                    className={`mylist-pagination-button ${currentPage === index + 1 ? 'active' : ''}`}
                                    disabled={pageLoading}
                                >
                                    {index + 1}
                                </button>
                            ))}
                            
                            <button 
                                className="mylist-pagination-button mylist-next"
                                onClick={goToNextPage}
                                disabled={currentPage === totalPages || pageLoading}
                            >
                                &raquo;
                            </button>
                        </div>
                    )}
                </>
            )}
            
            {/* 수정 모달 */}
            {editingFile && (
                <EditModal 
                    isOpen={editModalOpen}
                    onClose={() => setEditModalOpen(false)}
                    file={editingFile}
                    onUpdate={handleUpdateFile}
                />
            )}

            {/* 상세보기 모달 */}
            {viewingFile && (
                <DetailModal
                    isOpen={detailModalOpen}
                    onClose={() => setDetailModalOpen(false)}
                    file={viewingFile}
                />
            )}
        </div>
    );
};

export default Mylist;

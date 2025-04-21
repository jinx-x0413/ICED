import React, { useState, useEffect, useMemo, useRef } from 'react';
import './css/main.css';

// 상세보기 모달 컴포넌트
const DetailModal = ({ isOpen, onClose, file }) => {
    if (!isOpen) return null;

    // 날짜 포맷 함수
    const formatDate = (dateString) => {
        if (!dateString) return '날짜 정보 없음';
        
        const date = new Date(dateString);
        return date.toLocaleString('ko-KR', {
            year: 'numeric',
            month: 'long',
            day: 'numeric',
            hour: '2-digit',
            minute: '2-digit'
        });
    };
    
    // 파일 크기 포맷
    const formatFileSize = (size) => {
        if (!size) return '크기 정보 없음';
        return `${size.toFixed(2)} MB`;
    };

    // 장바구니에 추가하는 함수
    const addToCart = async () => {
        try {
            const token = localStorage.getItem('accessToken');
            
            const response = await fetch('http://localhost:8080/api/cart/add', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': token.startsWith('Bearer ') ? token : `Bearer ${token}`
                },
                body: JSON.stringify({ fileId: file.id })
            });
            
            if (!response.ok) {
                const errorData = await response.json();
                throw new Error(errorData.message || '장바구니 추가 실패');
            }
            
            alert('파일이 장바구니에 추가되었습니다.');
        } catch (error) {
            console.error('장바구니 추가 오류:', error);
            alert(`이미 장바구니에 있는 파일입니다`);
        }
    };

    return (
        <div className="modal-overlay">
            <div className="modal-content detail-modal">
                <div className="modal-header">
                    <h2>파일 상세정보</h2>
                    <button className="close-button" onClick={onClose}>&times;</button>
                </div>
                
                <div className="detail-content">
                    <div className="detail-image">
                        {file.thumbnailUri ? (
                            <img 
                                src={file.thumbnailUri.startsWith('http')
                                    ? file.thumbnailUri
                                    : `http://localhost:8080${file.thumbnailUri}`}
                                alt={file.description || 'File thumbnail'} 
                            />
                        ) : (
                            <div className="no-thumbnail-large">
                                <span>No Image</span>
                            </div>
                        )}
                    </div>
                    
                    <div className="detail-info">
                        <h3>{file.description || '제목 없음'}</h3>
                        
                        <table className="detail-table">
                            <tbody>
                                <tr>
                                    <th>설명</th>
                                    <td>{file.description || '설명 없음'}</td>
                                </tr>
                                <tr>
                                    <th>파일 크기</th>
                                    <td>{formatFileSize(file.size)}</td>
                                </tr>
                                <tr>
                                    <th>업로드 날짜</th>
                                    <td>{formatDate(file.createdAt)}</td>
                                </tr>
                            </tbody>
                        </table>
                        
                        <div className="detail-actions">
                            <button 
                                className="action-button cart large" 
                                onClick={addToCart}
                            >
                                장바구니에 추가
                            </button>
                            <button 
                                className="action-button preview large" 
                                onClick={onClose}
                            >
                                닫기
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
};

const Main = () => {
    const [files, setFiles] = useState([]);
    const [sortOrder, setSortOrder] = useState('newest');
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);
    const [searchTerm, setSearchTerm] = useState('');
    const [pageLoading, setPageLoading] = useState(true);
    const [detailModalOpen, setDetailModalOpen] = useState(false); // 상세보기 모달 상태
    const [viewingFile, setViewingFile] = useState(null); // 상세보기 중인 파일

    // 페이지네이션 상태
    const [currentPage, setCurrentPage] = useState(1);
    const [itemsPerPage, setItemsPerPage] = useState(10); // 기본값 설정 (나중에 계산됨)
    const [pageData, setPageData] = useState([]); // 현재 페이지 데이터

    // 파일 리스트 컨테이너에 대한 ref
    const fileListRef = useRef(null);
    const containerRef = useRef(null);
    const initialized = useRef(false);
    const resizeTimeoutRef = useRef(null); // 리사이즈 타임아웃 참조
    
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
    
    // 모든 파일 데이터 가져오기
    useEffect(() => {
        const fetchFiles = async () => {
            try {
                setLoading(true);
                setPageLoading(true); // 스켈레톤 UI 활성화를 위함
                
                const response = await fetch('http://localhost:8080/api/files');
                
                if (!response.ok) {
                    throw new Error('서버 응답 오류: ' + response.status);
                }
                
                const data = await response.json();
                setFiles(data || []);
                setLoading(false);
                // setPageLoading(false)는 제거 - pageData가 설정된 후 자동으로 해제됨
            } catch (error) {
                console.error('Error fetching files:', error);
                setError(error.message);
                setLoading(false);
                setPageLoading(false); // 에러 시 스켈레톤 UI 비활성화
            }
        };

        fetchFiles();
    }, []);
    
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
    
    // 검색어 및 정렬 기준에 따라 파일 필터링
    const filteredFiles = useMemo(() => {
        // 검색어가 있으면 필터링
        const filtered = searchTerm
            ? files.filter(file => 
                (file.description && file.description.toLowerCase().includes(searchTerm.toLowerCase()))
            )
            : files;

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
    }, [files, searchTerm, sortOrder]);
    
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
    }, [searchTerm, itemsPerPage]); // itemsPerPage 의존성 추가
    
    // 그리드 크기 변경 시 페이지 리셋
    useEffect(() => {
        if (initialized.current) {
            setCurrentPage(1);
        }
    }, [itemsPerPage]); // itemsPerPage 의존성 추가
    
    // 페이지네이션 계산 - 0으로 나누기 방지
    const totalPages = itemsPerPage > 0 ? Math.ceil(filteredFiles.length / itemsPerPage) : 0;
    
    // 페이지 데이터 계산 - 페이지 로딩 상태 추가
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
                
                // 약간의 지연을 주어 로딩 상태가 보이도록 함 (실제 API 호출 시에는 제거 가능)
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
    
    // 파일 상세보기 모달 열기
    const handleViewDetails = (file) => {
        setViewingFile(file);
        setDetailModalOpen(true);
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
    // 선택된 파일 ID를 관리하는 state
    const [selectedFiles, setSelectedFiles] = useState([]);

    // 파일 선택/해제 토글 함수
    const toggleFileSelection = (fileId) => {
        if (selectedFiles.includes(fileId)) {
            setSelectedFiles(selectedFiles.filter(id => id !== fileId));
        } else {
            setSelectedFiles([...selectedFiles, fileId]);
        }
    };

    // 현재 페이지 항목만 선택/해제하되 다른 페이지 선택은 유지하는 함수
    const toggleSelectAll = () => {
        // 현재 페이지의 모든 파일 ID 배열
        const currentPageIds = pageData.map(file => file.id);
        
        // 현재 페이지 파일들이 모두 선택되었는지 확인
        const allCurrentSelected = currentPageIds.every(id => 
            selectedFiles.includes(id)
        );
        
        if (allCurrentSelected) {
            // 현재 페이지 파일들만 선택 해제 (다른 페이지 선택은 유지)
            setSelectedFiles(selectedFiles.filter(id => 
                !currentPageIds.includes(id)
            ));
        } else {
            // 현재 페이지 파일들을 선택에 추가 (중복 방지)
            const newSelected = [...selectedFiles];
            
            currentPageIds.forEach(id => {
                if (!newSelected.includes(id)) {
                    newSelected.push(id);
                }
            });
            
            setSelectedFiles(newSelected);
        }
    };

    // 장바구니에 추가하는 함수
    const addSelectedToCart = async () => {
        if (selectedFiles.length === 0) {
            alert('선택된 파일이 없습니다.');
            return;
        }
        
        try {
            // 저장된 JWT 토큰 가져오기
            const token = localStorage.getItem('accessToken'); // 토큰 저장 방식에 맞게 수정
            
            // 선택된 각 파일에 대해 API 호출
            for (const fileId of selectedFiles) {
                const response = await fetch('http://localhost:8080/api/cart/add', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                        'Authorization': token.startsWith('Bearer ') ? token : `Bearer ${token}`
                    },
                    body: JSON.stringify({ fileId })
                });
                
                if (!response.ok) {
                    const errorData = await response.json();
                    throw new Error(errorData.message || '장바구니 추가 실패');
                }
            }
            
            // 성공시 선택 초기화
            setSelectedFiles([]);
            alert(`${selectedFiles.length}개 파일이 장바구니에 추가되었습니다.`);
        } catch (error) {
            console.error('장바구니 추가 오류:', error);
            alert(`이미 장바구니에 있는 파일이 포함되어 있습니다`);
        }
    };

    // 단일 파일을 장바구니에 추가하는 함수
    const addToCartSingle = async (fileId) => {
        try {
            const token = localStorage.getItem('accessToken');
            
            const response = await fetch('http://localhost:8080/api/cart/add', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': token.startsWith('Bearer ') ? token : `Bearer ${token}`
                },
                body: JSON.stringify({ fileId })
            });
            
            if (!response.ok) {
                const errorData = await response.json();
                throw new Error(errorData.message || '장바구니 추가 실패');
            }
            
            alert('파일이 장바구니에 추가되었습니다.');
        } catch (error) {
            console.error('장바구니 추가 오류:', error);
            alert(`이미 장바구니에 있는 파일입니다`);
        }
    };
    // 모든 선택을 해제하는 함수
    const clearAllSelections = () => {
        setSelectedFiles([]);
    };

    return (
        <div className="main-container" ref={containerRef}>
            {/* 검색 및 정렬 영역 */}
            <div className="controls-container">
                <div className="left-controls">
                    <div className="search-box">
                        <input
                            type="text"
                            placeholder="파일 검색..."
                            value={searchTerm}
                            onChange={handleSearch}
                        />
                        <span className="search-icon">🔍</span>
                    </div>
                    
                    <div className="sort-control">
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
                
                <div className="right-controls">
                <label className="select-all-control">
                    <input 
                        type="checkbox"
                        // 현재 페이지의 모든 아이템이 선택된 경우에만 체크됨
                        checked={pageData.length > 0 && pageData.every(file => 
                            selectedFiles.includes(file.id)
                        )}
                        onChange={toggleSelectAll}
                    />
                    <span>현재 페이지 전체 선택</span>
                </label>

                {selectedFiles.length > 0 && (
                    <>
                        <button 
                            className="action-button clear-selection-button"
                            onClick={clearAllSelections}
                        >
                            전체 선택 해제
                        </button>
                        
                        <button 
                            className="action-button cart-button"
                            onClick={addSelectedToCart}
                        >
                            장바구니에 추가 ({selectedFiles.length})
                        </button>
                    </>
                )}
            </div>
            </div>


            {/* 상태 표시 */}
            <div className="status-bar">
                <span>총 {filteredFiles.length}개의 파일</span>
            </div>

            {error ? (
                <div className="error-message">
                    <i className="error-icon">❌</i>
                    <p>파일 목록을 불러오는데 실패했습니다: {error}</p>
                </div>
            ) : filteredFiles.length === 0 && !loading && !pageLoading ? (
                <div className="empty-state">
                    <div className="empty-icon">📂</div>
                    <h3>파일이 없습니다</h3>
                    <p>검색 조건에 맞는 파일이 없거나 아직 업로드된 파일이 없습니다.</p>
                </div>
            ) : (
                <>
                    <div ref={fileListRef} className="file-grid">
                        {pageLoading || loading ? (
                            // 페이지 로딩 중일 때 스켈레톤 UI 표시
                            Array.from({ length: itemsPerPage || 6 }).map((_, index) => (
                                <div className="file-item skeleton" key={`skeleton-${index}`}>
                                    <div className="file-thumbnail skeleton-image"></div>
                                    <div className="file-info">
                                        <div className="skeleton-text"></div>
                                        <div className="skeleton-text skeleton-text-short"></div>
                                        <div className="file-actions">
                                            <div className="skeleton-button"></div>
                                        </div>
                                    </div>
                                </div>
                            ))
                        ) : (
                            <>                                
                                {/* 파일 항목들 */}
                                {pageData.map((file, index) => (
                                <div 
                                    className={`file-item ${selectedFiles.includes(file.id) ? 'selected' : ''}`} 
                                    key={index}
                                >
                                    <div className="file-checkbox">
                                        <input 
                                            type="checkbox"
                                            checked={selectedFiles.includes(file.id)}
                                            onChange={() => toggleFileSelection(file.id)}
                                            id={`file-checkbox-${file.id}`}
                                        />
                                        <label htmlFor={`file-checkbox-${file.id}`} className="checkbox-label"></label>
                                    </div>
                                    <div 
                                        className="file-thumbnail"
                                        onClick={() => toggleFileSelection(file.id)} // 썸네일 클릭 시 토글
                                    >
                                        <img 
                                            src={file.thumbnailUri} 
                                            alt={file.description || 'Unknown file'} 
                                        />
                                        {selectedFiles.includes(file.id) && (
                                            <div className="selected-overlay">
                                                <span className="selected-icon">✓</span>
                                            </div>
                                        )}
                                    </div>
                                    <div className="file-info">
                                        <p className="file-description">{file.description || 'No description'}</p>
                                        <div className="file-details">
                                            <span className="file-size">{file.size ? `${file.size.toFixed(2)} MB` : 'Unknown size'}</span>
                                            <span className="file-date">{formatDate(file.createdAt)}</span>
                                        </div>
                                        <div className="file-actions">
                                            <button 
                                                className="action-button preview"
                                                onClick={() => handleViewDetails(file)}
                                            >
                                                상세 보기
                                            </button>
                                            <button 
                                                className="action-button cart"
                                                onClick={() => addToCartSingle(file.id)}
                                            >
                                                장바구니
                                            </button>
                                        </div>
                                    </div>
                                </div>
                            ))}

                            </>
                        )}
                    </div>

                    
                    {/* 페이지네이션 UI */}
                    {totalPages > 1 && (
                        <div className="pagination">
                            <button 
                                className="pagination-button prev"
                                onClick={goToPreviousPage}
                                disabled={currentPage === 1 || pageLoading}
                            >
                                &laquo;
                            </button>
                            
                            {[...Array(totalPages)].map((_, index) => (
                                <button
                                    key={index}
                                    onClick={() => paginate(index + 1)}
                                    className={`pagination-button ${currentPage === index + 1 ? 'active' : ''}`}
                                    disabled={pageLoading}
                                >
                                    {index + 1}
                                </button>
                            ))}
                            
                            <button 
                                className="pagination-button next"
                                onClick={goToNextPage}
                                disabled={currentPage === totalPages || pageLoading}
                            >
                                &raquo;
                            </button>
                        </div>
                    )}
                </>
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

export default Main;
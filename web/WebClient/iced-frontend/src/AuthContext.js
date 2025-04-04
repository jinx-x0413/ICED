import { createContext, useContext, useEffect, useReducer } from "react";

// 초기 상태
const initialState = { user: null };

// 리듀서 함수
const authReducer = (state, action) => {
  switch (action.type) {
    case "LOGIN":
      return { ...state, user: action.payload };
    case "LOGOUT":
      return { ...state, user: null };
    default:
      return state;
  }
};

// Context 생성
const AuthContext = createContext();

// Context를 사용할 수 있도록 제공하는 컴포넌트
export const AuthProvider = ({ children }) => {
  const [state, dispatch] = useReducer(authReducer, initialState);

  useEffect(() => {
    const token = localStorage.getItem("accessToken");
    autoLogin(token, dispatch);
  }, []);

  return (
    <AuthContext.Provider value={{ state, dispatch }}>
      {children}
    </AuthContext.Provider>
  );
};

// JWT 디코딩 함수
const decodeJWT = (token) => {
  try {
    const temp = token.split(' ')[1]
    const base64Url = temp.split('.')[1]; // JWT의 페이로드 부분을 분리
    const base64 = base64Url.replace(/-/g, '+').replace(/_/g, '/'); // Base64Url을 Base64로 변환
    const jsonPayload = decodeURIComponent(atob(base64).split('').map(function(c) {
      return '%' + ('00' + c.charCodeAt(0).toString(16)).slice(-2);
    }).join(''));

    return JSON.parse(jsonPayload);
  } catch (error) {
    console.error("JWT 디코딩 실패:", error);
    return null;
  }
};

// 자동 로그인 함수
const autoLogin = async (token, dispatch) => {
  try {
    let res;

    if (!token) {
      // 토큰이 없는 경우, 토큰 발급 시도
      res = { status: 401 }; // 401 상태로 간주하여 토큰을 요청하도록 함
    } else {
      // 토큰이 있는 경우, 로그인 시도
      res = await fetch("http://localhost:8080/admin", {
        method: "GET",
        headers: { Authorization: token },
        credentials: "include",
      });
    }

    if (res.ok) {
      const decodedToken = decodeJWT(token);
      dispatch({ type: "LOGIN", payload: { userid: decodedToken.userid} });
    } else if (res.status === 401) {
      // 인증 실패 시, 토큰 갱신 시도
      const refreshRes = await fetch("http://localhost:8080/reissue", {
        method: "POST",
        credentials: "include",
      });

      if (refreshRes.ok) {
        const newToken = refreshRes.headers.get("Authorization");
        const decodedToken = decodeJWT(newToken);
        if (newToken) {
          localStorage.setItem("accessToken", newToken);
          dispatch({ type: "LOGIN", payload: { userid: decodedToken.userid } });
        }
      }
    }
  } catch (error) {
    console.error("로그인을 다시 해주세요:", error);
  }
};

// Context를 사용할 때 쓰는 훅
export const useAuth = () => useContext(AuthContext);

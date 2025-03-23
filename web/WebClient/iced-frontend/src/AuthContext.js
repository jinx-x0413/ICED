// AuthContext.js
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
    if (token) {
      autoLogin(token, dispatch);
    }
  }, []);

  return (
    <AuthContext.Provider value={{ state, dispatch }}>
      {children}
    </AuthContext.Provider>
  );
};

// 자동 로그인 함수
const autoLogin = async (token, dispatch) => {
  try {
    const res = await fetch("http://localhost:8080/admin", {
      method: "GET",
      headers: { Authorization: token },
      credentials: "include",
    });

    if (res.ok) {
      dispatch({ type: "LOGIN", payload: { userid: "자동 로그인 사용자" } });
    } else if (res.status === 401) {
      const refreshRes = await fetch("http://localhost:8080/reissue", {
        method: "POST",
        credentials: "include",
      });

      if (refreshRes.ok) {
        const newToken = refreshRes.headers.get("Authorization");
        if (newToken) {
          localStorage.setItem("accessToken", newToken);
          dispatch({ type: "LOGIN", payload: { userid: "자동 로그인 사용자" } });
        }
      }
    }
  } catch (error) {
    console.error("자동 로그인 실패:", error);
  }
};

// Context를 사용할 때 쓰는 훅
export const useAuth = () => useContext(AuthContext);

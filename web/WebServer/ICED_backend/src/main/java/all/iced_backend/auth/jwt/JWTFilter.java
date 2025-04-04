package all.iced_backend.auth.jwt;

import all.iced_backend.auth.dto.CustomUserDetails;
import all.iced_backend.auth.entity.Users;
import io.jsonwebtoken.*;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.filter.OncePerRequestFilter;

import java.io.IOException;
import java.io.PrintWriter;

public class JWTFilter extends OncePerRequestFilter {

    private final JWTUtil jwtUtil;

    public JWTFilter(JWTUtil jwtUtil) {
        this.jwtUtil = jwtUtil;
    }

    @Override
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {

        // 헤더에서 access 키에 담긴 토큰을 꺼냄
        String authorization = request.getHeader("Authorization");

        if (authorization == null || !authorization.startsWith("Bearer ")) {

            System.out.println("token null");
            filterChain.doFilter(request, response);

            //조건이 해당되면 메소드 종료 (필수)
            return;
        }

        System.out.println("authorization now");
        //Bearer 부분 제거 후 순수 토큰만 획득
        String accessToken = authorization.split(" ")[1];

        try {
            // 토큰 만료 여부 확인
            jwtUtil.isExpired(accessToken);

            // 토큰이 access인지 확인
            String category = jwtUtil.getCategory(accessToken);
            if (!"Authorization".equals(category)) {
                sendErrorResponse(response, HttpServletResponse.SC_UNAUTHORIZED, "invalid access token");
                return;
            }

            // userid, role 값 획득
            String userid = jwtUtil.getUserid(accessToken);
            String role = jwtUtil.getRole(accessToken);

            // 유저 정보 설정
            Users users = new Users();
            users.setUserid(userid);
            users.setRole(role);
            CustomUserDetails customUserDetails = new CustomUserDetails(users);

            // Spring Security 컨텍스트에 인증 정보 저장
            Authentication authToken = new UsernamePasswordAuthenticationToken(customUserDetails, null, customUserDetails.getAuthorities());
            SecurityContextHolder.getContext().setAuthentication(authToken);

            filterChain.doFilter(request, response);

        } catch (ExpiredJwtException e) {
            sendErrorResponse(response, HttpServletResponse.SC_UNAUTHORIZED, "access token expired");
        } catch (MalformedJwtException e) {
            sendErrorResponse(response, HttpServletResponse.SC_BAD_REQUEST, "invalid JWT format");
        } catch (SignatureException e) {
            sendErrorResponse(response, HttpServletResponse.SC_BAD_REQUEST, "invalid JWT signature");
        } catch (UnsupportedJwtException e) {
            sendErrorResponse(response, HttpServletResponse.SC_BAD_REQUEST, "unsupported JWT token");
        } catch (IllegalArgumentException e) {
            sendErrorResponse(response, HttpServletResponse.SC_BAD_REQUEST, "JWT token is null");
        } catch (Exception e) {
            sendErrorResponse(response, HttpServletResponse.SC_BAD_REQUEST, "unknown JWT error");
        }
    }

    // 에러 응답을 JSON 형식으로 반환하는 메서드
    private void sendErrorResponse(HttpServletResponse response, int status, String message) throws IOException {
        response.setStatus(status);
        response.setContentType("application/json");
        response.setCharacterEncoding("UTF-8");
        PrintWriter writer = response.getWriter();
        writer.print("{\"error\": \"" + message + "\"}");
        writer.flush();
    }
}

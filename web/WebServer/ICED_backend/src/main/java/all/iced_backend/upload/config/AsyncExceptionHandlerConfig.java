package all.iced_backend.upload.config;
import org.springframework.aop.interceptor.AsyncUncaughtExceptionHandler;
import org.springframework.context.annotation.Configuration;
import org.springframework.scheduling.annotation.AsyncConfigurer;

import java.lang.reflect.Method;

@Configuration
public class AsyncExceptionHandlerConfig implements AsyncConfigurer {

    @Override
    public AsyncUncaughtExceptionHandler getAsyncUncaughtExceptionHandler() {
        return new SilentAsyncExceptionHandler();
    }

    static class SilentAsyncExceptionHandler implements AsyncUncaughtExceptionHandler {
        @Override
        public void handleUncaughtException(Throwable ex, Method method, Object... params) {
            // 클라이언트 연결 끊김으로 인한 예외 등 무시
            if (ex.getClass().getName().contains("ClientAbortException") ||
                    ex.getClass().getName().contains("DisconnectedClientException")) {
                // 무시
                return;
            }

            // 그 외 예외는 로깅 (원한다면)
            System.err.println("비동기 처리 중 예외: " + ex.getMessage());
        }
    }
}
